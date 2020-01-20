#include <stdio.h>
#include <math.h>
#include <real.h>
#include <co/array.h>
#include <co/err.h>
#include <co/he.h>
#include <co/matrix.h>
#include <co/tensor3x3.h>
#include <co/y.h>

#define FMT CO_REAL_OUT

int
main(void)
{
    int n;
    Tensor3x3 q;

    n = 2;
    tensor3x3_ini(n, &q);
    tensor3x3_fin(q);
}
