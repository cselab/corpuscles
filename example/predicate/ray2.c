#include <stdio.h>
#include <real.h>

#include <co/predicate.h>
#include <co/vec2.h>

#define FMT CO_REAL_OUT

int
main(int argc, const char *v[])
{
	real a[2], b[2], d[2], e[2], x;
	predicate_ini();

	v++;
	vec2_argv(&v, a);
	vec2_argv(&v, b);
	vec2_argv(&v, d);

	vec2_argv(&v, e);

	printf("%d\n", predicate_ray2(d, e, a, b));
	return 0;
}


/*

./ray2   0.5 -1  0.5 1     0 0   1 0

*/
