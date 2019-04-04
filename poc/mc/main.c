#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>

#include <real.h>

#include <co/array.h>
#include <co/err.h>
#include <co/memory.h>
#include <co/macro.h>
#include <co/punto.h>
#include <co/kernel.h>
#include <co/cell3.h>

#include <alg/rng.h>

static AlgRng *rng;

int
main(void)
{
	alg_rng_ini(&rng);
}
