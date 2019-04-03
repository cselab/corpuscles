#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>

#include <real.h>
#include <co/err.h>
#include <co/vec.h>
#include <co/tri.h>
#include <co/macro.h>

#define FMT CO_REAL_OUT
static const char **argv;

static void
vec(/**/ real a[3])
{
	vec_argv(&argv, a);
}

int
main(__UNUSED int argc, const char **v0)
{
	real a[3], b[3], c[3];
	real p[3], q[3];

	argv = v0;
	argv++;
	vec(a);
	vec(b);
	vec(c);
	vec(p);
	tri_point_closest(a, b, c, p, q);
	vec_printf(q, FMT);
	return 0;
}
