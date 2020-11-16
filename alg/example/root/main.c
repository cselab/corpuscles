#include <stdio.h>
#include <real.h>
#include <co/err.h>

#include <alg/root.h>

#define FMT CO_REAL_OUT

static real
f(real x, void *p)
{
    real alpha;

    alpha = *(real *) p;
    return (x - alpha) * (x + 2);
}

int
main(void)
{
    AlgRoot *q;
    real a, b, r, alpha;

    alg_root_ini(BRENT, &q);
    a = -1;
    b = 1;
    alpha = 0.01;
    alg_root_apply(q, a, b, f, &alpha, &r);
    printf(FMT "\n", r);
    fprintf(stderr, "%s %d\n", alg_root_name(q), alg_root_niter(q));

    alg_root_fin(q);
}
