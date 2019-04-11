#include <stdio.h>
#include <stdlib.h>
#include <real.h>

#include <co/arg.h>
#include <co/err.h>
#include <co/integral/sph_plane.h>

#define FMT CO_REAL_OUT

int
main(void)
{
	real r, d, volume;
	SphPlane *integ;

	r = 1.0;
	d = 0.1;
	sph_plane_ini(r, &integ);
	sph_plane_volume(integ, d, &volume);

	printf(FMT "\n", volume);
	sph_plane_fin(integ);
}
