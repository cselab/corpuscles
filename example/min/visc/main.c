#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include <real.h>

#include <he/err.h>
#include <he/punto.h>
#include <he/macro.h>
#include <he/util.h>
#include <he/memory.h>
#include <he/x.h>

#include <alg/x.h>
#include <alg/min.h>

#define FMT_IN   XE_REAL_IN

static real Ka, Kga, Kv, Ke, Kb;
static real A0, V0;
static const char **argv;
static char bending[4048];
static const char *me = "min/kantor";

enum {KANTOR, GOMPPER};
static int btype;
static int f_bending_ini(const char *bending, real K) {
    if (util_eq(bending, "kantor")) {
        btype = KANTOR;
        f_kantor_ini(K);
    }
    else if (util_eq(bending, "gompper")) {
        btype = GOMPPER;
        f_gompper_ini(K);
    } else
        ER("unknown bending type: %s", bending);
    return HE_OK;
}

static int f_bending_fin() {
    switch (btype) {
    case KANTOR: return f_kantor_fin();
    case GOMPPER: return f_gompper_fin();
    }
    ER("unknown btype: %d", btype);
}

static real f_bending_energy(const real *x, const real *y, const real *z) {
    switch (btype) {
    case KANTOR: return f_kantor_energy(x, y, z);
    case GOMPPER: return f_gompper_energy(x, y, z);
    }
    ER("unknown btype: %d", btype);
}

static real f_bending_force(const real *x, const real *y, const real *z,
                            /**/ real *fx, real *fy, real *fz) {
    switch (btype) {
    case KANTOR: return f_kantor_force(x, y, z, /**/ fx, fy, fz);
    case GOMPPER: return f_gompper_force(x, y, z, /**/ fx, fy, fz);
    }
    ER("unknown btype: %d", btype);
}


static void usg() {
    fprintf(stderr, "%s kantor/gompper Ka Kga Kv Kb Ke < OFF > PUNTO\n", me);
    exit(0);
}

int eq(const char *a, const char *b) { return util_eq(a, b); }
int scl(/**/ real *p) {
    if (*argv == NULL) ER("not enough args");
    if (sscanf(*argv, FMT_IN, p) != 1)
        ER("not a number '%s'", *argv);
    argv++;
    return HE_OK;
}
int str(/**/ char *p) {
    if (*argv == NULL) ER("not enough args");
    strncpy(p, *argv, 4048);
    argv++;
    return HE_OK;
}
static void arg() {
    if (*argv != NULL && eq(*argv, "-h")) usg();
    str(bending);
    scl(&Ka); scl(&Kga); scl(&Kv); scl(&Kb); scl(&Ke);
}

real Energy(const real *x, const real *y, const real *z) {
    real a, ga, v, e, b;
    a = f_area_energy(x, y, z);
    ga = f_garea_energy(x, y, z);
    v = f_volume_energy(x, y, z);
    e = f_harmonic_energy(x, y, z);
    b = f_bending_energy(x, y, z);
    return a + ga + v + e + b;
}

static void zero(int n, real *a) {
    int i;
    for (i = 0; i < n; i++) a[i] = 0;
}
void Force(const real *x, const real *y, const real *z, /**/
           real *fx, real *fy, real *fz) {
    zero(NV, fx); zero(NV, fy); zero(NV, fz);
    f_area_force(x, y, z, /**/ fx, fy, fz);
    f_garea_force(x, y, z, /**/ fx, fy, fz);
    f_volume_force(x, y, z, /**/ fx, fy, fz);
    f_harmonic_force(x, y, z, /**/ fx, fy, fz);
    f_bending_force(x, y, z, /**/ fx, fy, fz);
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
                      real *vx, real *vy, real *vz, /*io*/
                      real *fx, real *fy, real *fz) {
    int i;
    for (i = 0; i < NV; i++) {
        fx[i] -= mu*vx[i];
        fy[i] -= mu*vy[i];
        fz[i] -= mu*vz[i];
    }
}

static void main0(real *vx, real *vy, real *vz,
                  real *fx, real *fy, real *fz) {
    int i;
    real dt, mu;
//    real *queue[] = {XX, YY, ZZ, NULL};
    i = 0;
    dt = 1e-5;
    mu = 1.0;
    zero(NV, vx); zero(NV, vy); zero(NV, vz);
    for (;;) {
        i++;
        Force(XX, YY, ZZ, /**/ fx, fy, fz);
        visc_lang(mu, vx, vy, vz, /**/
                  fx, fy, fz);
        euler( dt, vx, vy, vz, /**/ XX, YY, ZZ);
        euler(-dt, fx, fy, fz, /**/ vx, vy, vz);
        if (i % 100 == 0) {
            MSG("eng: %g", Energy(XX, YY, ZZ));
            MSG("%g %g", area()/A0, volume()/V0);
            off_write(XX, YY, ZZ, "q.off");
            MSG("dump: q.off");            
        }
    }
}

static real eq_tri_edg(real area) {
    /* area = sqrt(3)/4 * edg^2 */
    return 2*sqrt(area)/pow(3, 0.25);
}

static real rbc(real area) { return 0.06064602170131934*pow(area, 1.5); }
static real sph(real area) { return 0.09403159725795977*pow(area, 1.5); }

int main(int __UNUSED argc, const char *v[]) {
    real e0, a0;
    real *fx, *fy, *fz;
    real *vx, *vy, *vz;
    argv = v; argv++;
    arg();

    ini("/dev/stdin");
    A0 = area();
    a0 = A0/NT;
    V0 = rbc(A0); e0 = eq_tri_edg(a0);
    MSG("v0/volume(): %g", V0/volume());
    MSG("area, volume, edg: %g %g %g", A0, V0, e0);

    f_area_ini(a0,  Ka);
    f_garea_ini(A0, Kga);
    f_volume_ini(V0, Kv);
    f_harmonic_ini(e0, Ke);
    f_bending_ini(bending, Kb);

    MALLOC(NV, &fx); MALLOC(NV, &fy); MALLOC(NV, &fz);
    MALLOC(NV, &vx); MALLOC(NV, &vy); MALLOC(NV, &vz);

    main0(vx, vy, vz, fx, fy, fz);

    FREE(fx); FREE(fy); FREE(fz);
    FREE(vx); FREE(vy); FREE(vz);

    f_bending_fin();
    f_harmonic_fin();
    f_volume_fin();
    f_area_fin();
    f_garea_fin();
    fin();

    return 0;
}
