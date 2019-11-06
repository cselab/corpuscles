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
const real eps = 0.1;

static real
sq(real x)
{
	return x*x;
}


static real
wd(real r)
{
	return sq(1 - r/R);
}

static real
G(real x, real y, real z, void *p)
{
	real r, d, d0;
	d = *(real*)p;
	d0 = d/R < eps ? R*eps : d;
	r = sqrt(sq(x) + sq(y) + sq(z));
	return wd(r)/r*(1 - z/d0);
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
		sph_plane_apply(integ, d, G, &d, &res);
		printf(FMT "\n", res);
	}
	sph_plane_fin(integ);
}
