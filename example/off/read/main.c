#include <stdio.h>

#include <real.h>
#include <co/array.h>
#include <co/he.h>
#include <co/y.h>

int
main(void)
{
    real *x, *y, *z, hi;
    He *he;
    int nv, nt;

    y_inif(stdin, &he, &x, &y, &z);
    nv = he_nv(he);
    nt = he_nt(he);
    hi = array_max(nv, x);
    printf("number of vertices is %d\n"
           "number of triangles is %d\n"
           "maximum x coordinate is " CO_REAL_OUT "\n", nv, nt, hi);
    y_fin(he, x, y, z);
}
