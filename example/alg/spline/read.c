#include <stdio.h>
#include <tgmath.h>

#include <real.h>
#include <alg/spline.h>

#define FMT CO_REAL_OUT

int
main(void)
{
    int n, i;
    real x, y, lo, hi;
    AlgSpline *spline;

    alg_spline_fscanf(stdin, CSPLINE, &spline);
    n = 100;
    lo = alg_spline_lo(spline);
    hi = alg_spline_hi(spline);
    for (i = -10; i < n + 10; i++) {
        x = lo + (hi - lo) / (n - 1) * i;
        y = alg_spline_apply(spline, x);
        printf(FMT " " FMT "\n", x, y);
    }
    alg_spline_fin(spline);
}
