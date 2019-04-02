#include <stdio.h>
#include <real.h>

#include <co/predicate.h>
#include <co/vec.h>

#define FMT CO_REAL_OUT

enum
{
	X, Y, Z
};

int
main(int argc, const char *v[])
{
	real a[3], b[3], c[3], d[3], e[3], x;
	predicate_ini();

	v++;
	vec_argv(&v, a);
	vec_argv(&v, b);
	vec_argv(&v, c);
	vec_argv(&v, d);
	vec_argv(&v, e);

	printf("%d\n", predicate_ray(d, e, a, b, c));
	return 0;
}


/*

./ray 0 0 0    1 0 0    0 1 0      0.5 0.3 1    0.5 0.2 1

*/