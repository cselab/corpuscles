#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <real.h>

#include <co/err.h>
#include <co/integral/tri.h>
#include <co/macro.h>
#include <co/vec.h>

#define FMT CO_REAL_OUT

static real
f(real x, real y, real z, void *p)
{
    USED(p);
    return x*y*z;
}

int
main(__UNUSED int argc, const char **argv)
{
    real a[3], b[3], c[3], res;
    IntegralTri *q;

    argv++;
    vec_argv(&argv, a);
    vec_argv(&argv, b);
    vec_argv(&argv, c);
    integral_tri_ini(&q);
    integral_tri_apply(q, a, b, c, f, NULL, &res);
    printf(FMT "\n", res);
    integral_tri_fin(q);
}
