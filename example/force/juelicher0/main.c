#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include <real.h>

#include <he/f/juelicher.h>

#include <he/memory.h>
#include <he/punto.h>
#include <he/vec.h>
#include <he/err.h>
#include <he/macro.h>
#include <he/util.h>
#include <he/x.h>

#define FMT_IN   XE_REAL_IN

static real K, C0, Kad;
static real *FX, *FY, *FZ;
static const real Pi = 3.141592653589793115997964;
static const char **argv;

static const char *me = "force/juelicher";
static void usg() {
    fprintf(stderr, "%s K C0 Kad < OFF\n", me);
    exit(0);
}

int eq(const char *a, const char *b) { return util_eq(a, b); }
int scl(/**/ real *p) {
    if (*argv == NULL) {
        MSG("not enough args");
        usg();
    }
    if (sscanf(*argv, FMT_IN, p) != 1)
        ER("not a number '%s'", *argv);
    argv++;
    return HE_OK;
}
static void arg() {
    if (*argv != NULL && eq(*argv, "-h")) usg();
    scl(&K); scl(&C0); scl(&Kad);
}

static void zero(int n, real *a) {
    int i;
    for (i = 0; i < n; i++) a[i] = 0;
}
static void force() {
    zero(NV, FX); zero(NV, FY); zero(NV, FZ);
    f_juelicher_force(XX, YY, ZZ,   FX, FY, FZ);
}

static real energy() { return f_juelicher_energy(XX, YY, ZZ); }
static real energy_ver(real **p) { return f_juelicher_energy_ver(p); }
static real area_ver(real **p) { return f_juelicher_area_ver(p); }
static real curva_mean(real **p) { return f_juelicher_curva_mean(p); }

static void main0() {
    int i, nstep;
    real e0;
    real *eng, *area, *curv;

    e0 = energy(); force();
    e0 = energy(); force(); /* test */

    energy_ver(&eng);
    area_ver(&area);
    curva_mean(&curv);

    MSG("eng: %g", e0);
    MSG("FX[0]: %g", FX[0]);
    real *queue[] = {XX, YY, ZZ, FX, FY, FZ, area, eng, curv, NULL};
    punto_fwrite(NV, queue, stdout);
}

int main(int __UNUSED argc, const char *v[]) {
    argv = v; argv++;
    arg();
    ini("/dev/stdin");
    f_juelicher_ini(K, C0, Kad);
    MSG("Kad: %g", Kad);

    RZERO(NV, &FX); RZERO(NV, &FY); RZERO(NV, &FZ);
    main0();
    FREE(FX); FREE(FY); FREE(FZ);
    f_juelicher_fin();
    fin();
    return 0;
}
