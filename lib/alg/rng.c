#include <stdio.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

#include "real.h"
#include "co/err.h"
#include "co/memory.h"
#include "alg/rng.h"

#define T AlgRng

struct T {
    gsl_rng *r;
};

int
alg_rng_ini(T ** pq)
{
    T *q;
    gsl_rng *r;
    const gsl_rng_type *R;

    MALLOC(1, &q);
    R = gsl_rng_default;
    gsl_rng_env_setup();
    r = gsl_rng_alloc(R);
    q->r = r;
    *pq = q;
    return CO_OK;
}

int
alg_rng_fin(T * q)
{
    gsl_rng_free(q->r);
    FREE(q);
    return CO_OK;
}

real
alg_rng_gaussian(T * q, real sigma)
{
    double ans;

    ans = gsl_ran_gaussian(q->r, sigma);
    return ans;
}

real
alg_rng_uniform(T * q, real a, real b)
{
    return gsl_ran_flat(q->r, a, b);
}
