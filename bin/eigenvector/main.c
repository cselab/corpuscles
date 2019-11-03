#include <stdio.h>
#include <tgmath.h>
#include <real.h>
#include <co/eigen.h>
#include <co/err.h>
#include <co/he.h>
#include <co/macro.h>
#include <co/memory.h>
#include <co/off.h>
#include <co/y.h>

static int nv;
static real *x, *y, *z;
static real vec[3 * 3];
static He *he;
static Eigen *eigen;

static int
eigen_vector_point(Eigen * q, He * he, real * x, real * y, real * z,
                   real * v)
{
    USED(he);
    return eigen_vector(q, x, y, z, v);
}

int
main(int argc, char **argv)
{
    enum { POINT, SURFACE, TRI };
    int (*Alg)(Eigen *, He *, real *, real *, real *, real *);

    y_inif(stdin, &he, &x, &y, &z);
    nv = he_nv(he);
    eigen_ini(he, &eigen);

    argv++;
    Alg = eigen_vector_point;
    if (argv[0] && argv[0][0] == '-' && argv[0][1] == 's')
        Alg = eigen_vector_surface;
    if (argv[0] && argv[0][0] == '-' && argv[0][1] == 't')
        Alg = eigen_vector_tri;
    Alg(eigen, he, x, y, z, vec);
    printf("%.16g %.16g %.16g\n", vec[0], vec[3], vec[6]);
    eigen_fin(eigen);
    y_fin(he, x, y, z);
    return 0;
}
