#include <stdio.h>
#include <stdlib.h>
#include <real.h>
#include <co/err.h>
#include <co/generation.h>
#include <co/he.h>
#include <co/he/invariant.h>
#include <co/macro.h>
#include <co/memory.h>
#include <co/tri.h>
#include <co/vec.h>
#include <co/vtk.h>
#include <co/off.h>
#include <co/y.h>

static const char *me = "generation/read";
static void
usg()
{
    fprintf(stderr, "%s < VTK\n", me);
    exit(2);
}

int
main(int argc, char **argv)
{
    He *he;
    real *color;
    real *x;
    real *y;
    real *z;
    Generation *generation;
    int nt;

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

    if (generation_read(stdin, &he, &x, &y, &z, &generation) != CO_OK)
	ER("generation_read failed");
    if (he_invariant(he) != CO_OK)
        ER("he_invariant failed");
    generation_invariant(generation, he);
    generation_coarsen(generation, 3098, he, &x, &y, &z);
    generation_invariant(generation, he);
    he_invariant(he);
    nt = he_nt(he);
    MALLOC(nt, &color);
    generation_color(generation, he, color);
    boff_tri_fwrite(he, x, y, z, color, stdout);
    FREE(color);
    generation_fin(generation);
    he_fin(he);
    FREE3(x, y, z);
}
