#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include <real.h>

#include <co/err.h>
#include <co/punto.h>
#include <co/macro.h>
#include <co/util.h>
#include <co/x.h>

#include <alg/x.h>
#include <alg/min.h>

#define FMT_IN   CO_REAL_IN

static real rVolume, Ka, Kga, Kv, Ke, Kb;
static real A0, V0;
static const char **argv;
static char bending[4048];
static const char *me = "min/kantor";

enum {KANTOR, GOMPPER};
static int btype;
static int f_bending_ini(const char *bending, real K) {
    real C0, Kad, DA0D;
    if (util_eq(bending, "kantor")) {
        btype = KANTOR;
        f_kantor_ini(K);
    }
    else if (util_eq(bending, "gompper")) {
        btype = GOMPPER;
        C0 = Kad = DA0D = 0;
        f_gompper_ini(K, C0, Kad, DA0D);
    } else
        ER("unknown bending type: %s", bending);
    return CO_OK;
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


static void usg(void) {
    fprintf(stderr, "%s kantor/gompper rVolume Ka Kga Kv Kb Ke < OFF > PUNTO\n", me);
    exit(0);
}

int eq(const char *a, const char *b) { return util_eq(a, b); }
int scl(/**/ real *p) {
    if (*argv == NULL) ER("not enough args");
    if (sscanf(*argv, FMT_IN, p) != 1)
        ER("not a number '%s'", *argv);
    argv++;
    return CO_OK;
}
int str(/**/ char *p) {
    if (*argv == NULL) ER("not enough args");
    strncpy(p, *argv, 4048);
    argv++;
    return CO_OK;
}
static void arg() {
    if (*argv != NULL && eq(*argv, "-h")) usg();
    str(bending);
    scl(&rVolume); scl(&Ka); scl(&Kga); scl(&Kv); scl(&Kb); scl(&Ke);
}

real Energy(const real *x, const real *y, const real *z) {
    real a, ga, v, e, b;
    a = f_area_energy(x, y, z);
    ga = f_garea_energy(x, y, z);
    v = f_volume_energy(x, y, z);
    e = f_edg_sq_energy(x, y, z);
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
    f_edg_sq_force(x, y, z, /**/ fx, fy, fz);
    f_bending_force(x, y, z, /**/ fx, fy, fz);
}

static void main0() {
    int i;
    const real *queue[] = {XX, YY, ZZ, NULL};
//    punto_fwrite(NV, queue, stdout);
//    printf("\n");
    i = 0;
    for (;;) {
        i++;
        min_position(/**/ XX, YY, ZZ);
        if (i % 100 == 0) {
            punto_fwrite(NV, queue, stdout);
            printf("\n");
            MSG("eng: %g", min_energy());
            MSG("%g %g", area()/A0, volume()/V0);
            off_write(XX, YY, ZZ, "q.off");
            MSG("dump: q.off");
            if (min_end()) break;
        }
        min_iterate();
    }
}

static real sph_volume(real area) { return 0.09403159725795977*pow(area, 1.5); }
static real target_volume(real area, real v) { return v*sph_volume(area); }

int main(int __UNUSED argc, const char *v[]) {
    real a0;
    argv = v; argv++;
    arg();

    ini("/dev/stdin");
    A0 = area();
    a0 = A0/NT;
    V0 = target_volume(A0, rVolume);
    MSG("v0/volume(): %g", V0/volume());
    MSG("area, volume: %g %g", A0, V0);

    f_area_ini(a0,  Ka);
    f_garea_ini(A0, Kga);
    f_volume_ini(V0, Kv);
    f_edg_sq_ini(Ke);
    f_bending_ini(bending, Kb);

    min_ini(VECTOR_BFGS2);

    main0();
    min_fin();

    f_bending_fin();
    f_edg_sq_fin();
    f_volume_fin();
    f_area_fin();
    f_garea_fin();
    fin();
    return 0;
}
