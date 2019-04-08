#include <stdio.h>
#include <stdlib.h>
#include <real.h>

#include <co/arg.h>
#include <co/err.h>
#include <co/integral/sph_2plane.h>

#define FMT CO_REAL_OUT

char *argv0;
int
main(int argc, char **argv)
{
	real r, d0, d1, volume;
	Sph2Plane *integ;

	r = 2.0;
	d0 = 0.2;
	d1 = 0.4;
	sph_2plane_ini(r, &integ);
	sph_2plane_volume(integ, d0, d1, &volume);
	printf(FMT "\n", volume);
	sph_2plane_fin(integ);
}
