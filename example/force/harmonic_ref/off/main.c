#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include <real.h>

#include <co/err.h>
#include <co/f/harmonic_ref.h>
#include <co/he.h>
#include <co/macro.h>
#include <co/memory.h>
#include <co/punto.h>
#include <co/util.h>
#include <co/y.h>

static const char *me = "force/harmonic_ref/off";
static HeFHarmonicRef *force;
static He *he0, *he1;

static int nv;
static const char **argv;
static char off0[4048], off1[4048];
static real *x, *y, *z;

static void usg(void) {
    fprintf(stderr, "%s K ref.off main.off\n", me);
    exit(0);
}

static int str(/**/ char *p) {
    if (*argv == NULL) ER("not enough args");
    strncpy(p, *argv, 4048);
    argv++;
    return CO_OK;
}

static int scl(/**/ real *p) {
    if (*argv == NULL) ER("not enough args");
    if (sscanf(*argv, CO_REAL_IN, p) != 1)
        ER("not a number '%s'", *argv);
    argv++;
    return CO_OK;
}

static void main0() {
    real *fx, *fy, *fz;
    real en;

    CALLOC(nv, &fx);
    CALLOC(nv, &fy);
    CALLOC(nv, &fz);

    en = he_f_harmonic_ref_energy(force, he0, x, y, z);
    he_f_harmonic_ref_force(force, he0, x, y, z, /**/ fx, fy, fz);
    printf(CO_REAL_OUT "\n", en);

    char *key = "x y z fm fx fy fz";
    const real *queue[] = {x, y, z, fx, fy, fz, NULL};
    puts(key);
    punto_fwrite(nv, queue, stdout);

    FREE(fx); FREE(fy); FREE(fz);
}

int main(int __UNUSED argc, const char *v[]) {
    int nt;
    FILE *f;
    real *x0, *y0, *z0, K;

    if (*++v != NULL && util_eq(*v, "-h"))
        usg();
    argv = v;

    scl(&K);
    str(off0);
    str(off1);

    y_ini(off0, &he0, &x0, &y0, &z0);
    y_ini(off1, &he1, &x, &y, &z);

    nv = he_nv(he0);
    nt = he_nt(he0);

    if (nv != he_nv(he1))
        ER("nv=%d != he_nv(off0)=%d", nv, he_nv(he1));

    if (nt != he_nt(he1))
        ER("nt=%d != he_nt(off0)=%d", nt, he_nt(he1));

    he_f_harmonic_ref_ini(K, x0, y0, z0, he0, /**/ &force);

    main0();

    he_f_harmonic_ref_fin(force);
    y_fin(he0, x0, y0, z0);
    y_fin(he1, x, y, z);

    return 0;
}
