#include <stdio.h>
#include <stdlib.h>
#include <real.h>
#include <co/array.h>
#include <co/err.h>
#include <co/he.h>
#include <co/he/invariant.h>
#include <co/macro.h>
#include <co/memory.h>
#include <co/off.h>
#include <co/y.h>

static const char *me = "y/merge";
static void
usg(void)
{
    fprintf(stderr, "%s OFF OFF > OFF\n", me);
    exit(2);
}

int
main(int argc, char **argv)
{
    He *he, *he0, *he1;
    real *x, *y, *z, *x0, *y0, *z0, *x1, *y1, *z1;
    char *a, *b;
    int nv, nv0, nv1;
    FILE *fa, *fb;

    USED(argc);
    while (*++argv != NULL && argv[0][0] == '-')
        switch (argv[0][1]) {
        case 'h':
            usg();
            break;
        default:
            fprintf(stderr, "%s: unknown option '%s'\n", me, argv[0]);
            exit(2);
        }
    if ((a = *argv++) == NULL) {
        fprintf(stderr, "%s: needs OFF\n", me);
        exit(2);
    }
    if ((b = *argv++) == NULL) {
        fprintf(stderr, "%s: needs OFF\n", me);
        exit(2);
    }

    if ((fa = fopen(a, "r")) == NULL) {
        fprintf(stderr, "%s: fail to open '%s'\n", me, a);
        exit(2);
    }
    if ((fb = fopen(b, "r")) == NULL) {
        fprintf(stderr, "%s: fail to open '%s'\n", me, b);
        exit(2);
    }
    y_inif(fa, &he0, &x0, &y0, &z0);
    y_inif(fb, &he1, &x1, &y1, &z1);
    fclose(fa);
    fclose(fb);
    nv0 = he_nv(he0);
    nv1 = he_nv(he1);
    MSG("nv01: %d %d", nv0, nv1);
    he_merge2(he0, he1, &he);
    nv = he_nv(he);
    if (he_invariant(he) != CO_OK)
      ER("he_invariant failed");
    MALLOC3(nv, &x, &y, &z);
    array_copy3(nv0, x0, y0, z0, x, y, z);
    array_copy3(nv1, x1, y1, z1, &x[nv0], &y[nv0], &z[nv0]);
    boff_fwrite(he, x, y, z, stdout);
    y_fin(he, x, y, z);
    y_fin(he0, x0, y0, z0);
    y_fin(he1, x1, y1, z1);
}
