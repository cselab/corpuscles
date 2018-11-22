#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#include <real.h>

#include <he/err.h>
#include <he/punto.h>
#include <he/vec.h>
#include <he/macro.h>
#include <he/util.h>
#include <he/memory.h>
#include <he/tri.h>
#include <he/dedg.h>
#include <he/ddih.h>
#include <he/dtri.h>
#include <he/bending.h>
#include <he/x.h>
#include <alg/x.h>
#include <alg/min.h>

static const real pi = 3.141592653589793115997964;

static real Kb, C0, Kad, DA0D;
static void zero(int n, real *a) {
    int i;
    for (i = 0; i < n; i++) a[i] = 0;
}


#define FMT_IN   XE_REAL_IN

static real rVolume, Ka, Kga, Kv, Ke;
static int end;
static int freq;
static real A0, V0, e0;
static real et, eb, ek, ea, ega, ev, ee;
static const char **argv;
static char bending[4049];
static const char *me = "min/helfrich_xin_fga";

static void usg() {
    fprintf(stderr, "%s kantor/gompper/gompper_kroll/juelicher/juelicher_xin/meyer/meyer_xin rVolume Ka Kga Kv Ke Kb C0 Kad DA0D < OFF > msg\n", me);
    fprintf(stderr, "end: number of iterations\n");
    fprintf(stderr, "freq: frequency of output off files\n");
    exit(0);
}

static real reduced_volume(real area, real volume) { return (6*sqrt(pi)*volume)/pow(area, 3.0/2); }

static int eq(const char *a, const char *b) { return util_eq(a, b); }
static int num(/**/ int *p) {
    if (*argv == NULL) {
        usg();
        ER("not enough args");
    }
    if (sscanf(*argv, "%d", p) != 1)
        ER("not a number '%s'", *argv);
    argv++;
    return HE_OK;
}
static int scl(/**/ real *p) {
    if (*argv == NULL) ER("not enough args");
    if (sscanf(*argv, FMT_IN, p) != 1)
        ER("not a number '%s'", *argv);
    argv++;
    return HE_OK;
}
static int str(/**/ char *p) {
    if (*argv == NULL) ER("not enough args");
    strncpy(p, *argv, 4048);
    argv++;
    return HE_OK;
}
static void arg() {
    if (*argv != NULL && eq(*argv, "-h")) usg();
    str(bending);
    scl(&rVolume);
    scl(&Ka);
    scl(&Kga);
    scl(&Kv);
    scl(&Ke);
    scl(&Kb);
    scl(&C0);
    scl(&Kad);
    scl(&DA0D);
    num(&end);
    num(&freq);
}

real Energy(const real *x, const real *y, const real *z) {
    real a, ga, v, e, b;
    a = f_area_energy(x, y, z);
    ga = f_garea_energy(x, y, z);
    v = f_volume_energy(x, y, z);
    e = f_edg_sq_energy(x, y, z);
    b = f_bending_energy(x, y, z);


    et  = a + ga + v + e + b;
    ea  = a;
    ega = ga;
    ev  = v;
    ee  = e;
    eb  = b;

    return a + ga + v + e + b;
}

void Force(const real *x, const real *y, const real *z, /**/
           real *fx, real *fy, real *fz) {
    zero(NV, fx); zero(NV, fy); zero(NV, fz);
    f_area_force(x, y, z, /**/ fx, fy, fz);
    f_garea_force(x, y, z, /**/ fx, fy, fz);
    f_volume_force(x, y, z, /**/ fx, fy, fz);
    f_edg_sq_force(x, y, z, /**/ fx, fy, fz);
    f_bending_force(x, y, z, /**/ fx, fy, fz);
}
void ForceArea(const real *x, const real *y, const real *z, /**/
           real *fx, real *fy, real *fz) {
    zero(NV, fx); zero(NV, fy); zero(NV, fz);
    //f_area_force(x, y, z, /**/ fx, fy, fz);
    f_garea_force(x, y, z, /**/ fx, fy, fz);
}

static void euler(real dt,
                  const real *fx, const real *fy, const real *fz,
                  real *vx, real *vy, real *vz) {
    int i;
    for (i = 0; i < NV; i++) {
        vx[i] += dt*fx[i];
        vy[i] += dt*fy[i];
        vz[i] += dt*fz[i];
    }
}

static void visc_lang(real mu,
                      const real *vx, const real *vy, const real *vz, /*io*/
                      real *fx, real *fy, real *fz) {
    int i;
    for (i = 0; i < NV; i++) {
        fx[i] -= mu*vx[i];
        fy[i] -= mu*vy[i];
        fz[i] -= mu*vz[i];
    }
}

static void visc_pair(real mu,
                      const real *vx, const real *vy, const real *vz, /*io*/
                      real *fx, real *fy, real *fz) {
    int e, i, j;
    real a[3], b[3], u[3], u0;
    for (e = 0; e < NE; e++) {
        i = D1[e]; j = D2[e];
        vec_get(i, vx, vy, vz, a);
        vec_get(j, vx, vy, vz, b);
        vec_minus(a, b, u);
        u0 = vec_abs(u);
        vec_scalar_append(u, -mu*u0, i, fx, fy, fz);
        vec_scalar_append(u,  mu*u0, j, fx, fy, fz);
    }
}

static real Kin(real *vx, real *vy, real *vz) {
    int i;
    real s;
    s = 0;
    for (i = 0; i < NV; i++) {
        s += vx[i]*vx[i];
        s += vy[i]*vy[i];
        s += vz[i]*vz[i];
    }
    return s;
}

static real max_vec(real *fx, real *fy, real *fz) {
    int i;
    real c, m;
    m = 0;
    for (i = 0; i < NV; i++) {
        c = sqrt(fx[i]*fx[i] + fy[i]*fy[i] + fz[i]*fz[i]);
        if (c > m)
            m = c;
    }
    return m;
}

static int equi() {
    int cnt, j;
    for (j = 0; cnt > 0 && j < 1000; j++) {
        equiangulate(&cnt);
        if (cnt > 10) MSG("cnt : %d", cnt);
    }
}

static void main0(real *vx, real *vy, real *vz,
                  real *fx, real *fy, real *fz) {
  int cnt, i, j;
  real dt, dt_max, h, mu, rnd;
  real A, V, Vr;
  real *queue[] = {XX, YY, ZZ, NULL};
  int nsub;
  char file[4048];

  dt_max = 0.001;
  mu     = 1.0;
  h      = 0.01*e0;

  nsub = 10;

  zero(NV, vx); zero(NV, vy); zero(NV, vz);
  for (i = 0; i <= end; i++) {
    equi();
    Force(XX, YY, ZZ, /**/ fx, fy, fz);
    dt = fmin(dt_max,  sqrt(h/max_vec(fx, fy, fz)));
    visc_pair(mu, vx, vy, vz, /**/ fx, fy, fz);
    euler(-dt, vx, vy, vz, /**/ XX, YY, ZZ);
    euler( dt, fx, fy, fz, /**/ vx, vy, vz);

    for (j=0; j < nsub; j++ ) {
      equi();
      ForceArea(XX, YY, ZZ, /**/ fx, fy, fz);
      visc_pair(mu, vx, vy, vz, /**/ fx, fy, fz);
      euler(-dt, vx, vy, vz, /**/ XX, YY, ZZ);
      euler( dt, fx, fy, fz, /**/ vx, vy, vz);
    }

    if ( i % 100 == 0 ) {
      et = Energy(XX, YY, ZZ);
      //eb = f_bending_energy(XX, YY, ZZ);
      ek = Kin(vx, vy, vz);
      et = et + ek;
      A = area(); V = volume(); Vr=reduced_volume(A,V);
      MSG("eng: %g %g %g %g %g %g %g", et, eb, ea, ega, ev, ek, ee);
      MSG("dt: %g", dt);
      MSG("A/A0, V/V0, Vr: %g %g %g", A/A0, V/V0, Vr);
      printf("eng: %g %g %g %g %g %g %g\n", et, eb, ea, ega, ev, ek, ee);
      printf("dt: %f\n", dt);
      printf("A/A0, V/V0, Vr: %g %g %g\n", A/A0, V/V0, Vr);
    }

    if ( i % freq == 0 ) {
      sprintf(file, "%08d.off", i);
      off_write(XX, YY, ZZ, file);
    }
  }
}

static real sph_volume(real area) { return 0.09403159725795977*pow(area, 1.5); }
static real target_volume(real area, real v) { return v*sph_volume(area); }
static real eq_tri_edg(real area) { return 2*sqrt(area)/pow(3, 0.25); }


int main(int __UNUSED argc, const char *v[]) {
  real a0;
  real *fx, *fy, *fz;
  real *vx, *vy, *vz;
  real A, V, Vr;
  BendingParam bending_param;

  argv = v; argv++;
  arg();
  srand(time(NULL));

  ini("/dev/stdin");
  A0 = area();
  a0 = A0/NT;
  V0 = target_volume(A0, rVolume);
  e0 = eq_tri_edg(a0);

  A = A0;
  V = volume();
  Vr= reduced_volume(A, V);

  MSG("Targeted Area, Volume: %g %g", A0, V0);
  MSG("V/V0: %g", V/V0);
  MSG("A/A0: %g", A/A0);
  MSG("Vr  : %g", Vr);

  f_area_ini(a0,  Ka);
  f_garea_ini(A0, Kga);
  f_volume_ini(V0, Kv);
  f_edg_sq_ini(Ke);

  bending_param.Kb = Kb;
  bending_param.C0 = C0;
  bending_param.Kad = Kad;
  bending_param.DA0D = DA0D;
  f_bending_ini(bending, bending_param);

  MALLOC(NV, &fx); MALLOC(NV, &fy); MALLOC(NV, &fz);
  MALLOC(NV, &vx); MALLOC(NV, &vy); MALLOC(NV, &vz);

  main0(vx, vy, vz, fx, fy, fz);

  FREE(fx); FREE(fy); FREE(fz);
  FREE(vx); FREE(vy); FREE(vz);

  f_bending_fin();
  f_edg_sq_fin();
  f_volume_fin();
  f_area_fin();
  f_garea_fin();
  fin();

  return 0;
}