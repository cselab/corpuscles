#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <real.h>

#include <co/err.h>
#include <co/macro.h>
#include <co/util.h>
/* #include <co/x.h> */

#include <alg/x.h>
#include <alg/min.h>

#define FMT_IN   CO_REAL_IN

static real Ka, Kv, Ke, Kb;
static real A0, V0;
static const char **argv;
static const char *me = "min/gompper";

static void usg(void) {
    fprintf(stderr, "%s Ka Kv Kb Ke < OFF\n", me);
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
static void arg() {
    if (*argv != NULL && eq(*argv, "-h")) usg();
    scl(&Ka); scl(&Kv); scl(&Kb); scl(&Ke);
}

real Energy(const real *x, const real *y, const real *z) {
    real a, v, e, b;
    a = f_garea_energy(x, y, z);
    v = f_volume_energy(x, y, z);
    e = f_harmonic_energy(x, y, z);
    b = f_gompper_energy(x, y, z);
    return a + v + e + b;
}

static void zero(int n, real *a) {
    int i;
    for (i = 0; i < n; i++) a[i] = 0;
}
void Force(const real *x, const real *y, const real *z, /**/
           real *fx, real *fy, real *fz) {
    zero(NV, fx); zero(NV, fy); zero(NV, fz);
    f_garea_force(x, y, z, /**/ fx, fy, fz);
    f_volume_force(x, y, z, /**/ fx, fy, fz);
    f_harmonic_force(x, y, z, /**/ fx, fy, fz);
    f_gompper_force(x, y, z, /**/ fx, fy, fz);
}

static void main0() {
    int i;
    i = 0;
    while (!min_end()) {
        i++;
        min_position(/**/ XX, YY, ZZ);
        if (i % 100 == 0) {
            MSG("eng: %.16e", min_energy());
            off_write(XX, YY, ZZ, "q.off");
            MSG("%g %g", area()/A0, volume()/V0);
            MSG("dump: q.off");
        }
        min_iterate();
    }
}

static real eq_tri_edg(real area) {
    /* area = sqrt(3)/4 * edg^2 */
    return 2*sqrt(area)/pow(3, 0.25);
}

static real area2volume(real area) { return 0.06064602170131934*pow(area, 1.5); }

int main(int __UNUSED argc, const char *v[]) {
    real a0, e0;
    argv = v; argv++;
    arg();

    ini("/dev/stdin");
    A0 = area();
    a0 = A0/NT;   V0 = area2volume(A0); e0 = eq_tri_edg(a0);
    MSG("v0/volume(): %g", V0/volume());

    MSG("K[vabe] %g %g %g %g", Ka, Kv, Kb, Ke);
    MSG("[Vae]0  %g %g %g", V0, a0, e0);

    f_volume_ini(V0, Kv);
    f_garea_ini(A0, Ka);
    f_harmonic_ini(e0, Ke);
    f_gompper_ini(Kb);

    min_ini(VECTOR_BFGS2);

    main0();
    min_fin();

    f_gompper_fin();
    f_harmonic_fin();
    f_volume_fin();
    f_garea_fin();
    fin();
    return 0;
}
