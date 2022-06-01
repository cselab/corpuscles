#include <stdlib.h>
#include <stdio.h>
#include <real.h>
#include <co/he.h>
#include <co/xdmf.h>
#include <co/y.h>

static const char *me = "co.2xdmf";
static void
usg(void)
{
    fprintf(stderr, "%s < OFF > STL\n", me);
    exit(1);
}

int
main(int argc, char **argv)
{
    real *x;
    real *y;
    real *z;
    He *he;
    int i;
    int v;
    int nv;
    char xmf[FILENAME_MAX];
    char xyz[FILENAME_MAX];

    while (*++argv != NULL && argv[0][0] == '-')
        switch (argv[0][1]) {
        case 'h':
            usg();
            break;
        default:
            fprintf(stderr, "%s: unknown option '%s'\n", me, argv[0]);
            exit(1);
        }

    y_inif(stdin, &he, &x, &y, &z);
    xdmf_write(he, x, y, z, "a.0000.xdmf2", "0.ijk.raw", "0.xyz.raw");
    nv = he_nv(he);
    for (i = 1; i < 10; i++) {
        sprintf(xmf, "a.%04d.xdmf2", i);
        sprintf(xyz, "%04d.xyz.raw", i);
        for (v = 0; v < nv; v++) {
            x[v] *= 1.05;
            y[v] *= 0.95;
        }
        xdmf_xwrite(he, x, y, z, xmf, "0.ijk.raw", xyz);
    }

    y_fin(he, x, y, z);
    return 0;
}
