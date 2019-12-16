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

static const char *me = "generation/main";
static void
usg()
{
    fprintf(stderr, "%s OFF OFF > OFF\n", me);
    exit(2);
}

int
main(int argc, char **argv)
{
    He *he;
    real *x;
    real *y;
    real *z;

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
    y_inif(stdin, &he, &x, &y, &z);
    y_fin(he, x, y, z);
}
