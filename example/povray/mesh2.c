#include <stdio.h>
#include <stdlib.h>
#include <real.h>
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

static const char *me = "povray/mesh2";
static void
usg(void)
{
    fprintf(stderr, "%s < OFF > POV\n", me);
    exit(2);
}

int
main(int __UNUSED argc, const char **argv)
{
    He *he;
    real *x, *y, *z;

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
    povray_mesh2(he, x, y, z, stdout);
    y_fin(he, x, y, z);
}
