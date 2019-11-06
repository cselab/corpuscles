#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <real.h>
#include <co/err.h>
#include <co/vec2.h>
#include <co/dvec2.h>
#include <co/macro.h>
#include <co/util.h>

#define FMT_IN   CO_REAL_IN
static const char **argv;

static void
vec(/**/ real a[2])
{
	vec2_argv(&argv, a);
}

static int eq(const char *a, const char *b) { return util_eq(a, b); }
int main(__UNUSED int argc, const char **v) {
	const char *op;
	real a[2], b[2], da[2], db[2];
	argv = v;
	argv++;
	if (*argv == NULL) ER("mssing OP");

	op = *argv++;
	if (eq(op, "cross")) {
		vec(a);
		vec(b);
		dvec2_cross(a, b, da, db);
		vec2_fprintf(da, stdout);
		vec2_fprintf(db, stdout);
	} else
		ER("unknown operation '%s'", op);
	return 0;
}
