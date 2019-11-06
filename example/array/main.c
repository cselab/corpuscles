#include <stdio.h>
#include <math.h>
#include <real.h>
#include <co/err.h>
#include <co/array.h>

#define FMT CO_REAL_OUT

int
main(void)
{
	int n;
	real s;
	real x[] = {1, 2, 3};
	real y[] = {10, 20, 30};
	real z[] = {100, 200, 300};
	real u[] = {-1, -2, -3};
	real v[] = {-10, -20, -30};
	real w[] = {-100, -200, -300};

	n = sizeof(x)/sizeof(x[0]);
	s = array_diff3(n, x, y, z, u, v, w);
	printf(FMT "\n", s);
}
