#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#include <real.h>

#include <co/bending.h>
#include <co/err.h>
#include <co/f/strain.h>
#include <co/macro.h>
#include <co/memory.h>
#include <co/strain.h>
#include <co/util.h>
#include <co/vec.h>
#include <co/x.h>

static const real pi = 3.141592653589793115997964;
static const real EPS = 1e-6;

static real Kb, C0, Kad, DA0D;
static void zero(int n, real *a) {
    int i;
    for (i = 0; i < n; i++) a[i] = 0;
}


#define FMT_IN   CO_REAL_IN

static real rVolume, Ka, Kga, Kh, Ke;
static real A0, V0, e0;
static const char **argv;
static char bending[4049], model[4049], off[4049];
static const char *me = "min/visc";

static StrainParam strain_param;
static HeFStrain *strain;

static void usg(void) {
    fprintf(stderr, "%s %s \\\n"
            "  rVolume Ka Kga {off skalak/linear Ks Ka} {Kh Ke} {Kb C0 Kad DA0D} < OFF\n", me, bending_list());
    exit(0);
}

static real sph_volume(real area) { return pow(area, 1.5)/(6*pi); }
static real target_volume(real area, real v) { return v*sph_volume(area); }
static real target_area(real volume, real v) {
    real C;
    C = pow(3, 2/3.0) * pow(4, 1/3.0) * pow(pi, 1/3.0);
    return C*pow(volume/v, 2.0/3);
}
static real reduced_volume(real area, real volume) { return (6*sqrt(pi)*volume)/pow(area, 3.0/2); }
static real eq_tri_edg(real area) { return 2*sqrt(area)/pow(3, 0.25); }

static int eq(const char *a, const char *b) { return util_eq(a, b); }
static int scl(/**/ real *p) {
    if (*argv == NULL) ER("not enough args");
    if (sscanf(*argv, FMT_IN, p) != 1)
        ER("not a number '%s'", *argv);
    argv++;
    return CO_OK;
}
static int str(/**/ char *p) {
    if (*argv == NULL) ER("not enough args");
    strncpy(p, *argv, 4048);
    argv++;
    return CO_OK;
}
static void arg() {
    if (*argv != NULL && eq(*argv, "-h")) usg();
    str(bending);
    scl(&rVolume); scl(&Ka); scl(&Kga);
    str(off); str(model); scl(&strain_param.Ks); scl(&strain_param.Ka);
    scl(&Kh); scl(&Ke);
    scl(&Kb); scl(&C0);  scl(&Kad); scl(&DA0D);
}

real Energy(const real *x, const real *y, const real *z) {
    real a, ga, v, s, h, e, b;
    a = f_area_energy(x, y, z);
    ga = f_garea_energy(x, y, z);
    v = f_volume_energy(x, y, z);
    s = he_f_strain_energy(strain, x, y, z);
    h = f_harmonic_ref_energy(x, y, z);
    e = f_edg_sq_energy(x, y, z);
    b = f_bending_energy(x, y, z);
    MSG("a ga v s h e b: %g %g %g %g %g %g %g", a, ga, v, s, h, e, b);
    return a + ga + v + s + h + e + b;
}

void Force(const real *x, const real *y, const real *z, /**/
           real *fx, real *fy, real *fz) {
    zero(NV, fx); zero(NV, fy); zero(NV, fz);
    f_area_force(x, y, z, /**/ fx, fy, fz);
    f_garea_force(x, y, z, /**/ fx, fy, fz);
    he_f_strain_force(strain, x, y, z, /**/ fx, fy, fz);
    f_harmonic_ref_force(x, y, z, /**/ fx, fy, fz);
    f_edg_sq_force(x, y, z, /**/ fx, fy, fz);
    f_bending_force(x, y, z, /**/ fx, fy, fz);
}

void ForceVolume(const real *x, const real *y, const real *z, /**/
           real *fx, real *fy, real *fz) {
    zero(NV, fx); zero(NV, fy); zero(NV, fz);
    f_volume_force(x, y, z, /**/ fx, fy, fz);
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

static void jigle(real mag, /**/
                  real *x, real *y, real *z,
                  real *fx, real *fy, real *fz) {
    int i, j, e;
    real rnd, a[3], b[3], r[3], u[3];
    for (e = 0; e < NE; e++) {
        i = D1[e]; j = D2[e];
        vec_get(i, x, y, z, a);
        vec_get(j, x, y, z, b);
        vec_minus(a, b, r);
        vec_norm(r, u);
        rnd = mag * (rand()/(real)RAND_MAX - 0.5);
        vec_scalar_append(u,  rnd, i, fx, fy, fz);
        vec_scalar_append(u, -rnd, j, fx, fy, fz);
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

static void main0(real *vx, real *vy, real *vz,
                  real *fx, real *fy, real *fz) {
    int cnt, end, i, j, nsub, idump;
    real dt, dt_max, h, mu, T;
    real A, V;
    const real *queue[] = {XX, YY, ZZ, NULL};
    char file[4048];

    dt_max = 0.005;
    mu = 1000;
    h = 0.01*e0;
    end = 1000000;
    nsub = 1;
    T = 0;
    zero(NV, vx); zero(NV, vy); zero(NV, vz);
    for (idump = i = 0; i < end ; i++) {
        Force(XX, YY, ZZ, /**/ fx, fy, fz);
        dt = fmin(dt_max,  sqrt(h/max_vec(fx, fy, fz)));
        jigle(mu*T/sqrt(dt), XX, YY, ZZ, fx, fy, fz);
        visc_pair(mu, vx, vy, vz, /**/ fx, fy, fz);
        euler(-dt, vx, vy, vz, /**/ XX, YY, ZZ);
        euler( dt, fx, fy, fz, /**/ vx, vy, vz);

        for (j = 0; j < nsub; j++) {
            ForceVolume(XX, YY, ZZ, /**/ fx, fy, fz);
            jigle(mu*T/sqrt(dt), XX, YY, ZZ, fx, fy, fz);
            visc_pair(mu, vx, vy, vz, /**/ fx, fy, fz);
            euler(-dt, vx, vy, vz, /**/ XX, YY, ZZ);
            euler( dt, fx, fy, fz, /**/ vx, vy, vz);
        }

        if (i > 0 && i % 500 == 0) {
          do {
              equiangulate(&cnt);
              if (cnt > 10)
                  MSG("cnt : %d", cnt);
          } while (cnt > 0);
        }

        if (i % 100 == 0) {
            MSG("eng: %g", Energy(XX, YY, ZZ));
            A = area(); V = volume();
            MSG("area, vol, rVolume: %g %g %g", A/A0, V/V0, reduced_volume(A, V));
            sprintf(file, "%05d.off", idump++);
            off_write(XX, YY, ZZ, file);
        }

    }
}

int main(int __UNUSED argc, const char *v[]) {
    real a0, Kv;
    real *fx, *fy, *fz;
    real *vx, *vy, *vz;
    BendingParam bending_param;

    argv = v; argv++;
    arg();
    srand(time(NULL));

    ini("/dev/stdin");
    V0 = volume(); A0 = target_area(V0, rVolume);
    MSG("volume, area, target area: %g %g %g", V0, area(), A0);

    a0 = A0/NT;
    e0 = eq_tri_edg(a0);

    MSG("v0/volume(): %g", V0/volume());
    MSG("a0/area(): %g", A0/area());
    MSG("area, volume, edg: %g %g %g", A0, V0, e0);

    f_area_ini(a0,  Ka);
    f_garea_ini(A0, Kga);
    f_volume_ini(V0, Kv = 1);
    x_restore_ini(V0);
    he_f_strain_ini(off, model, strain_param, /**/ &strain);
    f_harmonic_ref_ini(Kh, XX, YY, ZZ);

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

    he_f_strain_fin(strain);
    f_bending_fin();
    x_restore_fin();
    f_volume_fin();
    f_area_fin();
    f_garea_fin();
    f_harmonic_ref_fin();
    f_edg_sq_fin();
    fin();

    return 0;
}
