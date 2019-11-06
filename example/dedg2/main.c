#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <real.h>
#include <co/dedg2.h>
#include <co/err.h>
#include <co/macro.h>
#include <co/util.h>
#include <co/vec2.h>

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
	real a[2], b[2], p[2], q[2], dp[2], dq[2], da[2], db[2];
	argv = v;
	argv++;
	if (*argv == NULL) ER("mssing OP");

	op = *argv++;
	if (eq(op, "abs")) {
		vec(a);
		vec(b);
		dedg2_abs(a, b, da, db);
		vec2_fprintf(da, stdout);
		vec2_fprintf(db, stdout);
	} else if (eq(op, "sq")) {
		vec(a);
		vec(b);
		dedg2_sq(a, b, da, db);
		vec2_fprintf(da, stdout);
		vec2_fprintf(db, stdout);
	} else if (eq(op, "alpha")) {
		vec(a);
		vec(b);
		vec(p);
		vec(q);
		dedg2_alpha(a, b, p, q, dp, dq);
		vec2_fprintf(dp, stdout);
		vec2_fprintf(dq, stdout);
	} else
		ER("unknown operation '%s'", op);
	return 0;
}
