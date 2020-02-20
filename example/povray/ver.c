#include <stdio.h>
#include <stdlib.h>
#include <real.h>
#include <co/array.h>
#include <co/err.h>
#include <co/he.h>
#include <co/he/invariant.h>
#include <co/macro.h>
#include <co/remesh.h>
#include <co/memory.h>
#include <co/povray.h>
#include <co/vec.h>
#include <co/tri.h>
#include <co/y.h>

static const char *me = "povray/ver";
static void
usg(void)
{
    fprintf(stderr, "%s < OFF > POV\n", me);
    exit(2);
}

int
main(int __UNUSED argc, const char **argv)
{
    enum {X, Y, Z};
    He *he;
    real *x, *y, *z, *ver;
    int nv;
    
    while (*++argv != NULL && argv[0][0] == '-')
        switch (argv[0][1]) {
        case 'h':
            usg();
            break;
        default:
            fprintf(stderr, "%s: unknown option '%s'\n", me, argv[0]);
            exit(2);
        }
    y_inif(stdin, &he, &x, &y, &z);
    nv = he_nv(he);
    MALLOC(nv, &ver);
    array_copy(nv, x, ver);
    povray_ver_mesh2(he, x, y, z, ver, stdout);
    y_fin(he, x, y, z);
    FREE(ver);
}
