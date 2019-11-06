#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <real.h>

#include <co/arg.h>
#include <co/err.h>
#include <co/integral/sph_plane.h>
#include <co/macro.h>

#define FMT CO_REAL_OUT

const real R = 1.0;

static real
sq(real x)
{
	return x*x;
}

static real
wc(real r)
{
	return 1 - r/R;
}

static real
E(real x, real y, real z, void __UNUSED *p)
{
	real r;
	r = sqrt(sq(x) + sq(y) + sq(z));
	return -wc(r)*z/r;
}


int
main(void)
{
	real d, lo, hi, res;
	int i, n;
	SphPlane *integ;
	sph_plane_ini(R, &integ);

	n = 200;
	lo = 0; 
	hi = 1;
	for (i = 0; i < n; i++) {
		d = lo + (hi - lo)/(n - 1)*i;
		sph_plane_apply(integ, d, E, NULL, &res);
		printf(FMT "\n", res);
	}
	sph_plane_fin(integ);
}
