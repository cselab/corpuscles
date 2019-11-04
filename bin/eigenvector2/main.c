#include <stdio.h>
#include <tgmath.h>

#include <real.h>
#include <co/memory.h>
#include <co/off.h>
#include <co/eigen.h>
#include <co/he.h>
#include <co/y.h>

static int nv;
static real *x, *y, *z;
static real vec[3 * 3];
static He *he;
static Eigen *eigen;

int
main(int argc, char **argv)
{
    int Surface;

    y_inif(stdin, &he, &x, &y, &z);
    nv = he_nv(he);
    eigen_ini(he, &eigen);

    argv++;
    Surface = 0;
    if (argv[0] && argv[0][0] == '-' && argv[0][1] == 's')
        Surface = 1;

    if (Surface)
        eigen_vector_surface(eigen, he, x, y, z, vec);
    else
        eigen_vector(eigen, x, y, z, vec);
    eigen_fin(eigen);

    printf("%g %g %g\n", vec[1], vec[4], vec[7]);

    y_fin(he, x, y, z);
    return 0;
}
