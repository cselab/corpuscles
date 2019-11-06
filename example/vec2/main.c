#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <real.h>
#include <co/err.h>
#include <co/vec2.h>
#include <co/macro.h>
#include <co/util.h>

#define FMT_IN   CO_REAL_IN
#define FMT CO_REAL_OUT
static const char **argv;

static void
vec(/**/ real a[2]) {
	vec2_argv(&argv, a);
}
static int
scl(/**/ real *p)
{
	if (*argv == NULL) ER("not enough args");
	if (sscanf(*argv, FMT_IN, p) != 1)
		ER("not a number '%s'", *argv);
	argv++;
	return CO_OK;
}

static int
eq(const char *a, const char *b)
{
	return util_eq(a, b);
}

int
main(__UNUSED int argc, const char **v)
{
	const char *op;
	real a[2], b[2], c[2], m[2], s;
	argv = v;
	argv++;
	if (*argv == NULL) ER("mssing OP");

	op = *argv++;
	if (eq(op, "cross")) {
		vec(a);
		vec(b);
		s = vec2_cross(a, b);
		printf(FMT "\n", s);
	} else if (eq(op, "abs")) {
		vec(a);
		s = vec2_abs(a);
		printf(FMT "\n", s);
	} else if (eq(op, "normalize")) {
		vec(a);
		vec2_normalize(a);
		vec2_fprintf(a, stdout);
	} else if (eq(op, "angle")) {
		vec(a);
		vec(b);
		printf(FMT "\n", vec2_angle(a, b));
	} else
		ER("unknown operation '%s'", op);
	return 0;
}


