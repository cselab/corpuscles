#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <real.h>
#include <co/err.h>
#include <co/vec.h>
#include <co/macro.h>
#include <co/util.h>

#include <co/edg.h>
#include <co/dedg.h>

#define FMT CO_REAL_OUT

static const char **argv;

static void
vec(/**/ real a[3])
{
	vec_argv(&argv, a);
}

static int
eq(const char *a, const char *b)
{
	return util_eq(a, b);
}

int main(__UNUSED int argc, const char **v)
{
	const char *op;
	real a[3], b[3], da[3], db[3], p[3], q[3];
	argv = v;
	argv++;
	if (*argv == NULL) ER("mssing OP");

	op = *argv++;
	if (eq(op, "abs")) {
		vec(a);
		vec(b);
		printf("%g\n", edg_abs(a, b));
	} else if (eq(op, "sq")) {
		vec(a);
		vec(b);
		printf("%g\n", edg_sq(a, b));
	} else if (eq(op, "dabs")) {
		vec(a);
		vec(b);
		dedg_abs(a, b, /**/ da, db);
		vec_printf(da, "%g");
		vec_printf(db, "%g");
	} else if (eq(op, "point_distance2")) {
		vec(a);
		vec(b);
		vec(p);
		printf(FMT "\n", edg_point_distance2(a, b, p));
	} else if (eq(op, "point_distance")) {
		vec(a);
		vec(b);
		vec(p);
		printf(FMT "\n", edg_point_distance(a, b, p));
	} else if (eq(op, "point_closest")) {
		vec(a);
		vec(b);
		vec(p);
		edg_point_closest(a, b, p, q);
		vec_printf(q, FMT);
	} else
		ER("unknown operation '%s'", op);
	return 0;
}
