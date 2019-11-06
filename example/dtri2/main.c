#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <real.h>
#include <co/err.h>
#include <co/vec2.h>
#include <co/dtri2.h>
#include <co/macro.h>
#include <co/util.h>

#define FMT_IN   CO_REAL_IN
static const char **argv;

static void
vec(/**/ real a[3])
{
	vec2_argv(&argv, a);
}
static int
scl(/**/ real *p) {
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
main(__UNUSED int argc, const char **v0)
{
	const char *op;
	real a[2], b[2], c[2], da[2], db[2], dc[2];

	argv = v0;
	argv++;
	if (*argv == NULL) ER("mssing OP");
	op = *argv++;
	if (eq(op, "angle")) {
		vec(a);
		vec(b);
		vec(c);
		dtri2_angle(a, b, c, da, db, dc);
		vec2_fprintf(da, stdout);
		vec2_fprintf(db, stdout);
		vec2_fprintf(dc, stdout);
	} else if (eq(op, "angle_sup")) {
		vec(a);
		vec(b);
		vec(c);
		dtri2_angle_sup(a, b, c, da, db, dc);
		vec2_fprintf(da, stdout);
		vec2_fprintf(db, stdout);
		vec2_fprintf(dc, stdout);
	} else
		ER("unknown operation '%s'", op);
	return 0;
}

