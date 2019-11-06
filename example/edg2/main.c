#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <real.h>
#include <co/edg2.h>
#include <co/err.h>
#include <co/macro.h>
#include <co/util.h>
#include <co/vec2.h>

#define FMT CO_REAL_OUT

static const char **argv;

static void
vec(/**/ real a[2])
{
	vec2_argv(&argv, a);
}

static int
eq(const char *a, const char *b)
{
	return util_eq(a, b);
}

int main(__UNUSED int argc, const char **v)
{
	const char *op;
	real a[2], b[2], da[2], db[2], p[2], q[2];
	argv = v;
	argv++;
	if (*argv == NULL) ER("mssing OP");

	op = *argv++;
	if (eq(op, "abs")) {
		vec(a);
		vec(b);
		printf(FMT "\n", edg2_abs(a, b));
	} else if (eq(op, "sq")) {
		vec(a);
		vec(b);
		printf("%g\n", edg2_sq(a, b));
	} else if (eq(op, "alpha")) {
		vec(a);
		vec(b);
		vec(p);
		vec(q);
		printf("%g\n", edg2_alpha(a, b, p, q));
	} else if (eq(op, "point_distance2")) {
		vec(a);
		vec(b);
		vec(p);
		printf(FMT "\n", edg2_point_distance2(a, b, p));
	} else if (eq(op, "point_distance")) {
		vec(a);
		vec(b);
		vec(p);
		printf(FMT "\n", edg2_point_distance(a, b, p));
	} else if (eq(op, "point_closest")) {
		vec(a);
		vec(b);
		vec(p);
		edg2_point_closest(a, b, p, q);
		vec2_fprintf(q, stdout);
	} else
		ER("unknown operation '%s'", op);
	return 0;
}
