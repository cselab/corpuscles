#include <stdio.h>
#include <real.h>

#include <alg/rng.h>

int
main(void)
{
    real a, b, sigma, u, g;
    int n;

    AlgRng *rng;

    alg_rng_ini(&rng);
    a = 2;
    b = 3;
    sigma = 5.0;
    n = 10;
    while (n--) {
        u = alg_rng_uniform(rng, a, b);
        g = alg_rng_gaussian(rng, sigma);
        printf(CO_REAL_OUT " " CO_REAL_OUT "\n", u, g);
    }
    alg_rng_fin(rng);
}
