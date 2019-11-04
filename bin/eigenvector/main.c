#include <stdio.h>
#include <stdlib.h>
#include <real.h>
#include <co/eigen.h>
#include <co/err.h>
#include <co/he.h>
#include <co/macro.h>
#include <co/vec.h>
#include <co/y.h>
#define FMT CO_REAL_OUT
const char me[] = "co.eigenvector";

static void
usg(void)
{
    fprintf(stderr, "%s [-sptv] < C.off\n", me);
    fprintf(stderr, "  -t   use triangles\n", me);
    fprintf(stderr, "  -s   use surface\n", me);
    fprintf(stderr, "  -p   use points (default)\n", me);
    fprintf(stderr, "  -a   print eigenvalues (default: eigenvector)\n",
            me);
    exit(2);
}

static int
eigen_vector_point(Eigen * q, He * he, real * x, real * y, real * z,
                   real * v)
{
    USED(he);
    return eigen_vector(q, x, y, z, v);
}

int
main(__UNUSED int argc, char **argv)
{
    enum { VECTOR, VALUE };
    int Report;
    real *x, *y, *z, vec[3 * 3], val[3];
    He *he;
    Eigen *eigen;

    int (*Alg)(Eigen *, He *, real *, real *, real *, real *);
    const char *arg;

    argv++;
    Alg = eigen_vector_point;
    Report = VECTOR;
    while (*argv != NULL) {
        arg = argv++[0];
        if (arg[0] != '-')
            break;
        switch (arg[1]) {
        case 'h':
            usg();
            break;
        case 's':
            Alg = eigen_vector_surface;
            break;
        case 'p':
            Alg = eigen_vector_point;
            break;
        case 't':
            Alg = eigen_vector_tri;
            break;
        case 'a':
            Report = VALUE;
            break;
        default:
            ER("%s: unknown option '%s'", me, arg);
        }
    }

    y_inif(stdin, &he, &x, &y, &z);
    eigen_ini(he, &eigen);
    Alg(eigen, he, x, y, z, vec);
    eigen_values(eigen, val);

    if (Report == VECTOR)
        printf(FMT " " FMT " " FMT "\n", vec[0], vec[3], vec[6]);
    else
        vec_printf(val, FMT);
    eigen_fin(eigen);
    y_fin(he, x, y, z);
    return 0;
}
