#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <real.h>

#include <he/err.h>
#include <he/punto.h>
#include <he/macro.h>
#include <he/util.h>
#include <he/x.h>

#include <alg/x.h>
#include <alg/min.h>

#define FMT_IN   XE_REAL_IN

static real Ka, Kv, Ke, Kb;
static const char **argv;
static const char *me = "min/kantor";

static void usg() {
    fprintf(stderr, "%s Ka Kv Kb Ke < OFF > PUNTO\n", me);
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
static void arg() {
    if (*argv != NULL && eq(*argv, "-h")) usg();
    scl(&Ka); scl(&Kv); scl(&Kb); scl(&Ke);
}

real Energy(const real *x, const real *y, const real *z) {
    real a, v, e, b;
    a = f_area_energy(x, y, z);
    v = f_volume_energy(x, y, z);
    e = f_harmonic_energy(x, y, z);
    b = f_kantor_energy(x, y, z);
    return a + v + e + b;
}

static void zero(int n, real *a) {
    int i;
    for (i = 0; i < n; i++) a[i] = 0;
}
void Force(const real *x, const real *y, const real *z, /**/
           real *fx, real *fy, real *fz) {
    zero(NV, fx); zero(NV, fy); zero(NV, fz);
    f_area_force(x, y, z, /**/ fx, fy, fz);
    f_volume_force(x, y, z, /**/ fx, fy, fz);
    f_harmonic_force(x, y, z, /**/ fx, fy, fz);
    f_kantor_force(x, y, z, /**/ fx, fy, fz);
}

static void main0() {
    int i;
    real *queue[] = {XX, YY, ZZ, NULL};
    punto_fwrite(NV, queue, stdout);
    printf("\n");
    for (i = 0; i < 1000000; i++) {
        min_position(/**/ XX, YY, ZZ);
        if (i % 100 == 0) {
            punto_fwrite(NV, queue, stdout);
            printf("\n");
            MSG("eng: %g", min_energy());
            off_write(XX, YY, ZZ, "q.off");
            MSG("dump: q.off");
        }
        min_iterate();
    }
}

static real eq_tri_edg(real area) {
    /* area = sqrt(3)/4 * edg^2 */
    return 2*sqrt(area)/pow(3, 0.25);
}

int main(int __UNUSED argc, const char *v[]) {
    real v0, a0, e0;
    argv = v; argv++;
    arg();

    ini("/dev/stdin");
    a0 = area()/NT; v0 = volume(); e0 = eq_tri_edg(a0);
    MSG("area, volume, edg: %g %g %g", a0, v0, e0);

    f_volume_ini(v0, Kv);
    f_area_ini(a0, Ka);
    f_harmonic_ini(e0, Ke);
    f_kantor_ini(Kb);

    min_ini(VECTOR_BFGS2);

    main0();
    min_fin();

    f_kantor_fin();
    f_harmonic_fin();
    f_volume_fin();
    f_area_fin();
    fin();
    return 0;
}
