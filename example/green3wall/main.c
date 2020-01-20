#include <stdio.h>
#include <math.h>
#include <real.h>
#include <co/array.h>
#include <co/err.h>
#include <co/he.h>
#include <co/matrix.h>
#include <co/memory.h>
#include <co/tensor3x3.h>
#include <co/green/3wall.h>
#include <co/y.h>

#define FMT CO_REAL_OUT

int
main(void)
{
    He *he;
    Green3Wall *green;
    real *x, *y, *z;
    real w;
    struct Tensor3x3 tensor;
    int n;

    w = -2.0;
    err_set_ignore();
    y_inif(stdin, &he, &x, &y, &z);
    n = he_nv(he);
    tensor3x3_ini(n, &tensor);
    green3_wall_ini(he, w, &green);
    green3_wall_apply(green, he, x, y, z, &tensor);

    green3_wall_fin(green);
    tensor3x3_fin(&tensor);
    y_fin(he, x, y, z);
}
