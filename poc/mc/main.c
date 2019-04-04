#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>

#include <real.h>

#include <co/array.h>
#include <co/err.h>
#include <co/memory.h>
#include <co/macro.h>
#include <co/kernel.h>

#include <alg/rng.h>

static AlgRng *rng;

int
main(void)
{
	real lo, hi;

	lo = -1; hi = 1;

	alg_rng_ini(&rng);
	alg_rng_fin(rng);
}
