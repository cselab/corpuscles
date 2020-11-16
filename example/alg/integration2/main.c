#include <stdio.h>
#include <real.h>

#include <alg/integration.h>
#include <alg/integration2.h>

#define FMT CO_REAL_OUT

static real
f(real x, real y, void *p)
{
    real alpha;

    alpha = *(real *) p;
    return alpha * x * y;
}

static real
zero(real x, void *p)
{
    return 0;
}

static real
one(real x, void *vp)
{
    real alpha;

    alpha = *(real *) vp;
    return alpha;
}

int
main(void)
{
    AlgIntegration2 *integ;
    real a, b, result, alpha;

    alg_integration2_ini(GAUSS31, &integ);
    a = 0;
    b = 1;
    alpha = 10;
    alg_integration2_apply(integ, a, b, &zero, &one, &f, &alpha, &result);
    printf(FMT "\n", result);
    alg_integration2_fin(integ);
}
