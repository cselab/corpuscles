#include <stdio.h>
#include <stdlib.h>
#include <real.h>

#include <co/arg.h>
#include <co/err.h>
#include <co/integral/circle_line.h>

#define FMT CO_REAL_OUT

int
main(void)
{
	err_set_ignore();
	real r, d, volume;
	CircleLine *integ;

	r = 2.0;
	d = 0;
	circle_line_ini(r, &integ);
	circle_line_volume(integ, d, &volume);

	printf(FMT "\n", volume);
	circle_line_fin(integ);
}
