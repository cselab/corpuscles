#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include <real.h>
#include <co/git.h>
#include <co/area.h>
#include <co/err.h>
#include <co/he.h>
#include <co/i/vec.h>
#include <co/i/tri.h>
#include <co/i/area.h>
#include <co/macro.h>
#include <co/memory.h>
#include <co/punto.h>
#include <co/y.h>

GIT_SYMBOL;
static char me[] = "area/ver";

static void
usg()
{
    fprintf(stderr, "%s < OFF\n", me);
    fprintf(stderr, "git: " GIT_STRING "\n");
    exit(2);
}

int
main(int argc, const char **argv)
{
    He *he;
    int n;
    real *x, *y, *z;
    real A;
    real *area;

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
    n = he_nv(he);
    MALLOC(n, &area);

    A = he_area(he, x, y, z);
    i_area_ver(he, x, y, z, /**/ area);

    MSG("area: " CO_REAL_OUT, A);

    const real *queue[] = { area, NULL };
    puts("area");
    punto_fwrite(n, queue, stdout);

    FREE(area);
    y_fin(he, x, y, z);
    return CO_OK;
}
