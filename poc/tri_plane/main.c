#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <real.h>
#include <co/err.h>
#include <co/green/3wall.h>
#include <co/he.h>
#include <co/macro.h>
#include <co/matrix.h>
#include <co/memory.h>
#include <co/ten.h>
#include <co/tensor3x3.h>
#include <co/y.h>

#define FMT CO_REAL_OUT
static char me[] = "poc/tri_plane";
static void
usg()
{
    fprintf(stderr, "%s [-w wall] < OFF\n", me);
    exit(1);
}

int
main(int argc, const char **argv)
{
    USED(argc);
    He *he;
    real *x;
    real *y;
    real *z;
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
    y_fin(he, x, y, z);
}
