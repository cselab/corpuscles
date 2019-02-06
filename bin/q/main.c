#include <stdio.h>
#include <stdlib.h>

#include <real.h>
#include <he/argv.h>
#include <he/area.h>
#include <he/err.h>
#include <he/off.h>
#include <he/he.h>
#include <he/macro.h>
#include <he/memory.h>
#include <he/util.h>

#include <he/y.h>

static char **argv;
static HeOff *off;
static int nt;
static real *x, *y, *z;
static He *he;
static real lo, hi, *a;

static const char *me = "he.q";
static void usg(void) {
    fprintf(stderr, "%s query [ARGS..] < IN.off > OUT.off\n", me);
    fputs("color off file\n", stderr);
    exit(2);
}

int main(__UNUSED int c, char **v) {
    char q[1024];
    argv = v;
    argv++;
    if (!argv[0] || util_eq(argv[0], "-h"))
        usg();
    argv_str(&argv, q);


    lo = 0; hi = 0.005;
    y_ini("/dev/stdin", &he, &x, &y, &z);

    nt = he_nt(he);
    CALLOC(nt, &a);

    he_area_tri(he, x, y, z, a);
    //boff_lh_tri_fwrite(he, x, y, z, lo, hi, a, stdout);
    boff_tri_fwrite(he, x, y, z, a, stdout);
    y_fin(he, x, y, z);

    FREE(a);
    return 0;
}
