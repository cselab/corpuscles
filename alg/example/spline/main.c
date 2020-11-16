#include <stdio.h>
#include <tgmath.h>
#include <real.h>

#include <co/memory.h>
#include <alg/spline.h>

#define FMT CO_REAL_OUT
#define PI (3.141592653589793)

static real
f(real x)
{
    return sin(x);
}

int
main(void)
{
    int n, m, i;
    real *x, *y, u, v;
    AlgSpline *spline;

    n = 10;
    m = 100;

    MALLOC(n, &x);
    MALLOC(n, &y);
    for (i = 0; i < n; i++) {
        x[i] = 2 * PI * i / (n - 1);
        y[i] = f(x[i]);
    }
    alg_spline_ini(n, x, y, CSPLINE, &spline);
    FREE(x);
    FREE(y);

    for (i = -10; i < m + 10; i++) {
        u = 2 * PI * i / (m - 1);
        v = alg_spline_apply(spline, u);
        printf(FMT " " FMT "\n", u, v);
    }
    alg_spline_fin(spline);
}
