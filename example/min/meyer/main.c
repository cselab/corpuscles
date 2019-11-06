#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#include <real.h>

#include <co/area.h>
#include <co/ddih.h>
#include <co/dedg.h>
#include <co/dtri.h>
#include <co/equiangulate.h>
#include <co/err.h>
#include <co/f/area.h>
#include <co/f/edg_sq.h>
#include <co/f/garea.h>
#include <co/filter.h>
#include <co/f/meyer.h>
#include <co/f/volume_normal.h>
#include <co/filter.h>
#include <co/he.h>
#include <co/macro.h>
#include <co/memory.h>
#include <co/off.h>
#include <co/punto.h>
#include <co/tri.h>
#include <co/util.h>
#include <co/vec.h>
#include <co/volume.h>
#include <co/vtk.h>
#include <co/y.h>

#define FMT_IN   CO_REAL_IN

static const real pi = 3.141592653589793115997964;
static const real tolerA = 1;

static real Kb;
static void zero(int n, real *a) {
    int i;
    for (i = 0; i < n; i++) a[i] = 0;
}

static HeFMeyer *f_bending;
static HeFEdgSq *f_edg_sq;
static HeFVolumeNormal *f_volume_normal;
static HeFGarea *f_garea;
static HeFArea *f_area;

static real rVolume, Ka, Kga, Kv, Ke;
static int end;
static int freq;
static real A0, V0, e0;
static real et, eb, ek, ea, ega, ev, ee;
static const char **argv;
static const char *me = "min/meyer";
static He *he;
static Filter *filter;
static real *XX, *YY, *ZZ, *fx, *fy, *fz, *fm;
static real *vx, *vy, *vz;
static real *fx, *fy, *fz;

static int NV, NE, NT;

static void usg(void) {
    fprintf(stderr, "%s rVolume Ka Kga Kv Ke Kb < OFF > msg\n", me);
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
    return CO_OK;
}
static int scl(/**/ real *p) {
    if (*argv == NULL) ER("not enough args");
    if (sscanf(*argv, FMT_IN, p) != 1)
        ER("not a number '%s'", *argv);
    argv++;
    return CO_OK;
}
static void arg() {
    if (*argv != NULL && eq(*argv, "-h")) usg();
    scl(&rVolume);
    scl(&Ka);
    scl(&Kga);
    scl(&Kv);
    scl(&Ke);
    scl(&Kb);
    num(&end);
    num(&freq);
}

real Energy(const real *x, const real *y, const real *z) {
    real a, ga, v, e, b;
    a = he_f_area_energy(f_area, he, x, y, z);
    ga = he_f_garea_energy(f_garea, he, x, y, z);
    v = he_f_volume_normal_energy(f_volume_normal, he, x, y, z);
    e = he_f_edg_sq_energy(f_edg_sq, he, x, y, z);
    b = he_f_meyer_energy(f_bending, he, x, y, z);

    et  = a + ga + v + e + b;
    ea  = a;
    ega = ga;
    ev  = v;
    ee  = e;
    eb  = b;
    
    return a + ga + v + e + b;
}

void Force(const real *x, const real *y, const real *z) {
    zero(NV, fx); zero(NV, fy); zero(NV, fz);
    he_f_area_force(f_area, he, x, y, z, /**/ fx, fy, fz);
    he_f_garea_force(f_garea, he, x, y, z, /**/ fx, fy, fz);
    he_f_volume_normal_force(f_volume_normal, he, x, y, z, /**/ fx, fy, fz);
    he_f_edg_sq_force(f_edg_sq, he, x, y, z, /**/ fx, fy, fz);
    he_f_meyer_force(f_bending, he, x, y, z, /**/ fx, fy, fz);
}
void ForceArea(const real *x, const real *y, const real *z, /**/
           real *fx, real *fy, real *fz) {
    zero(NV, fx); zero(NV, fy); zero(NV, fz);
    //f_area_force(x, y, z, /**/ fx, fy, fz);
    he_f_garea_force(f_garea, he, x, y, z, /**/ fx, fy, fz);
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

static void visc_pair(real mu,
                      const real *vx, const real *vy, const real *vz, /*io*/
                      real *fx, real *fy, real *fz) {
    int e, i, j;
    real a[3], b[3], u[3], u0;
    for (e = 0; e < NE; e++) {
        he_edg_ij(he, e, &i, &j);
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

static int filter0() {
    filter_apply(filter, he, XX, YY, ZZ, vx);
    filter_apply(filter, he, XX, YY, ZZ, vy);
    filter_apply(filter, he, XX, YY, ZZ, vz);
    return CO_OK;
}

static int step(real dt) {
    euler(-dt, vx, vy, vz, /**/ XX, YY, ZZ);
    euler( dt, fx, fy, fz, /**/ vx, vy, vz);
    filter0();
    return CO_OK;
}

static void main0() {
  int cnt, i, j;
  real dt, dt_max, h, mu;
  real A, V, Vr;
  real errA;
  int nsub;

  dt_max = 0.01;
  mu     = 0.0;
  h      = 0.01*e0;
  
  nsub = 100;
  zero(NV, vx); zero(NV, vy); zero(NV, vz);
  for (i = 0; i <= end; i++) {
    Force(XX, YY, ZZ);
    dt = fmin(dt_max,  sqrt(h/max_vec(fx, fy, fz)));
    visc_pair(mu, vx, vy, vz, /**/ fx, fy, fz);
    step(dt);
    
    j = 0;
    A  = he_area(he, XX, YY, ZZ);
    errA = (A-A0)/A0;
    if (errA<0) {
      errA=-errA;
    }
    
    while (j < nsub && errA > tolerA) {
      ForceArea(XX, YY, ZZ, /**/ fx, fy, fz);
      visc_pair(mu, vx, vy, vz, /**/ fx, fy, fz);
      step(dt);
      
      j++;
      A  = he_area(he, XX, YY, ZZ);
      errA = (A-A0)/A0;
      if (errA<0) {
	errA=-errA;
      }
    }

    int fr = 1;
    if (i > fr && i % fr == 0) {
        do {
            he_equiangulate(he, XX, YY, ZZ, &cnt);
            if (cnt > 10)
                MSG("cnt : %d", cnt);
        } while (cnt > 0);
    }

    if ( i % 100 == 0 ) {
      et = Energy(XX, YY, ZZ);
      ek = Kin(vx, vy, vz);
      et = et + ek;
      A = he_area(he, XX, YY, ZZ);
      V = he_volume_tri(he, XX, YY, ZZ);
      Vr=reduced_volume(A,V);
      MSG("eng: %g %g %g %g %g %g %g", et, eb, ea, ega, ev, ek, ee); 
      MSG("dt: %g", dt);
      MSG("A/A0, V/V0, Vr: %g %g %g", A/A0, V/V0, Vr);
      printf("eng: %g %g %g %g %g %g %g\n", et, eb, ea, ega, ev, ek, ee); 
      printf("dt: %f\n", dt);
      printf("A/A0, V/V0, Vr: %g %g %g\n", A/A0, V/V0, Vr);
    }
    
    if ( i % freq == 0 ) {
        real *f, *fa, *fb, *fc;
        int j;
        char off[4048], vtk[4048];
        sprintf(off, "%08d.off", i);
        sprintf(vtk, "%08d.vtk", i);
        off_he_xyz_write(he, XX, YY, ZZ, off);

        he_f_meyer_components(f_bending, &f, &fa, &fb, &fc);

        const real *scalars[] = {fx, fy, fz, f, fa, fb, fc, NULL};
        const char *names[]   = {"fx", "fy", "fz", "f", "fa", "fb", "fc", NULL};
        vtk_write(he, XX, YY, ZZ, scalars, names, vtk);
    }
  }
}

static real sph_volume(real area) { return 0.09403159725795977*pow(area, 1.5); }
static real target_volume(real area, real v) { return v*sph_volume(area); }
static real eq_tri_edg(real area) { return 2*sqrt(area)/pow(3, 0.25); }

int main(int __UNUSED argc, const char *v[]) {
  real a0;
  real A, V, Vr;
  
  argv = v; argv++;
  arg();
  srand(time(NULL));
  
  y_inif(stdin, &he, &XX, &YY, &ZZ);
  NV = he_nv(he);
  NE = he_ne(he);
  NT = he_nt(he);
  
  A0 = he_area(he, XX, YY, ZZ);
  a0 = A0/NT;
  V0 = target_volume(A0, rVolume);
  e0 = eq_tri_edg(a0);

  A = A0;
  V = he_volume_tri(he, XX, YY, ZZ);
  Vr= reduced_volume(A, V);
  
  MSG("Targeted Area, Volume: %g %g", A0, V0);
  MSG("V/V0: %g", V/V0);
  MSG("A/A0: %g", A/A0);
  MSG("Vr  : %g", Vr);
  
  he_f_area_ini(A0/NT, Ka, he, &f_area);
  he_f_garea_ini(A0, Kga, he, &f_garea);
  he_f_volume_normal_ini(V0, Kv, he, &f_volume_normal);
  he_f_edg_sq_ini(Ke, he, &f_edg_sq);
  he_f_meyer_ini(Kb, he, &f_bending);

  filter_ini(he, &filter);
  
  MALLOC(NV, &fx); MALLOC(NV, &fy); MALLOC(NV, &fz); MALLOC(NV, &fm);
  MALLOC(NV, &vx); MALLOC(NV, &vy); MALLOC(NV, &vz);
  
  main0();
  
  FREE(fx); FREE(fy); FREE(fz); FREE(fm);
  FREE(vx); FREE(vy); FREE(vz);

  filter_fin(filter);
  he_f_meyer_fin(f_bending);
  he_f_edg_sq_fin(f_edg_sq);
  he_f_volume_normal_fin(f_volume_normal);
  he_f_area_fin(f_area);
  he_f_garea_fin(f_garea);
  
  y_fin(he, XX, YY, ZZ);
  
  return 0;
}
