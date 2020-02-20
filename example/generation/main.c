#include <stdio.h>
#include <stdlib.h>
#include <real.h>
#include <co/err.h>
#include <co/he.h>
#include <co/he/invariant.h>
#include <co/macro.h>
#include <co/memory.h>
#include <co/vec.h>
#include <co/tri.h>
#include <co/vtk.h>
#include <co/y.h>
#include <co/generation.h>

static const char *me = "generation/main";
static void
usg(void)
{
    fprintf(stderr, "%s < OFF > OFF\n", me);
    exit(2);
}

static int tri_xyz(int t, He * he, const real * x, const real * y,
                   const real * z, real * u, real * v, real * w);

int
main(int argc, char **argv)
{
    He *he;
    real *color;
    real *x;
    real *y;
    real *z;
    int nt;
    int i;
    real u;
    real v;
    real w;
    Generation *generation;

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
    generation_ini(he, &generation);
    nt = he_nt(he);
    for (i = 0; i < nt; i++) {
        tri_xyz(i, he, x, y, z, &u, &v, &w);
        if (u > 0)
            generation_refine(generation, i, he, &x, &y, &z);
    }
    nt = he_nt(he);
    for (i = 0; i < nt; i++) {
        tri_xyz(i, he, x, y, z, &u, &v, &w);
        if (v > 0)
            generation_refine(generation, i, he, &x, &y, &z);
    }
    generation_invariant(generation, he);

    nt = he_nt(he);
    MALLOC(nt, &color);
    generation_color(generation, he, color);
    /* if (boff_tri_write(he, x, y, z, color, stdout) != CO_OK)
       ER("boff_tri_write failed"); */
    generation_write(generation, he, x, y, z, stdout);
    FREE(color);
    generation_fin(generation);
    y_fin(he, x, y, z);
}

static int
tri_xyz(int t, He * he, const real * x, const real * y, const real * z,
        real * u, real * v, real * w)
{
    enum { X, Y, Z };
    int i;
    int j;
    int k;
    real a[3];
    real b[3];
    real c[3];
    real d[3];

    he_tri_ijk(he, t, &i, &j, &k);
    vec_get3(i, j, k, x, y, z, a, b, c);
    tri_center(a, b, c, d);
    *u = d[X];
    *v = d[Y];
    *w = d[Z];
    return CO_OK;
}
