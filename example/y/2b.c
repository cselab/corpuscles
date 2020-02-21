#include <stdio.h>
#include <stdlib.h>
#include <real.h>
#include <co/err.h>
#include <co/he.h>
#include <co/he/multi.h>
#include <co/macro.h>
#include <co/memory.h>
#include <co/off.h>
#include <co/y.h>

static const char *me = "y/2b";
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
    real *x, *y, *z, *x1, *y1, *z1;
    char *a, *b;
    int nv0;
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
    y_inif2b(fa, fb, &he0, &he1, &he, &x, &y, &z);
    nv0 = he_nv(he0);
    x1 = &x[nv0];
    y1 = &y[nv0];
    z1 = &z[nv0];
    MSG("he_nv(he): %d", he_nv(he));
    MSG("he_nv(he0): %d", he_nv(he0));
    MSG("he_nv(he1): %d", he_nv(he1));
    boff_fwrite(he, x, y, z, stdout);
    y_fin2b(he0, he1, he, x, y, z);
    fclose(fa);
    fclose(fb);
}
