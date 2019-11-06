#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include <real.h>
#include <co/err.h>
#include <co/memory.h>
#include <co/macro.h>
#include <co/f/wlc.h>
#include <co/he.h>
#include <co/sum.h>
#include <co/util.h>
#include <co/y.h>

static const char *me = "force/wlc/off";
static HeFWlc *force;
static He *he0, *he1;

static int nv;
static const char **argv;
static char off0[4048], off1[4048];
static real *x, *y, *z;

static void usg(void) {
    fprintf(stderr, "%s x0 K ref.off main.off\n", me);
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

static real mean(int n, real *x) {
    return he_sum_array(n, x)/n;
}

static void main0() {
    real en;
    en = he_f_wlc_energy(force, he0, x, y, z);
    printf(CO_REAL_OUT "\n", en);
}

int main(int __UNUSED argc, const char *v[]) {
    int nt;
    FILE *f;
    real *x0, *y0, *z0, K, x00;

    if (*++v != NULL && util_eq(*v, "-h"))
        usg();
    argv = v;

    scl(&x00);
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

    he_f_wlc_ini(x00, K, x0, y0, z0, he0, /**/ &force);

    main0();

    he_f_wlc_fin(force);
    y_fin(he0, x0, y0, z0);
    y_fin(he1, x, y, z);

    return 0;
}
