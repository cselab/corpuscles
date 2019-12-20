#include <stdio.h>
#include <stdlib.h>
#include <real.h>
#include <co/err.h>
#include <co/he.h>
#include <co/macro.h>
#include <co/memory.h>
#include <co/off.h>
#include <co/vtk.h>

static const char *me = "vtk/int";
static void
usg()
{
    fprintf(stderr, "%s < VTK > OFF\n", me);
    exit(2);
}

int
main(int argc, char **argv)
{
    USED(argc);
    He *he;
    int i;
    int *id;
    int *neg;
    int n;
    real *x;
    real *y;
    real *z;

    while (*++argv != NULL && argv[0][0] == '-')
        switch (argv[0][1]) {
        case 'h':
            usg();
            break;
        }
    int **scalars[] = { &id, &neg, NULL };
    const char *names[] = { "id", "neg", NULL };
    vtk_tri_int_read(stdin, names, &he, &x, &y, &z, scalars);
    off_he_xyz_fwrite(he, x, y, z, stdout);
    he_fin(he);
    FREE3(x, y, z);
    FREE2(id, neg);
}
