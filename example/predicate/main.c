#include <stdio.h>
#include <real.h>
#include <co/predicate.h>

#define FMT CO_REAL_OUT

enum
{
	X, Y, Z
};

int
main(int argc, char **argv)
{
	real a[3], b[3], c[3], x;
	predicate_ini();

	a[X] = a[Y] = 0;
	b[X] = b[Y] = 1;
	c[X] = 1;
	c[Y] = 0.9;
	x = predicate_orient2d(a, b, c);
	printf(FMT "\n", x);
	return 0;
}