#include <stdio.h>
#include <tgmath.h>

#include "real.h"
#include "co/arc.h"
#include "co/err.h"
#include "co/macro.h"
#include "co/memory.h"
#include "co/skel.h"

#define T Skel
#define PI (3.141592653589793)


static real
x(real t, void *p)
{
	real b;

	b = *(real *) p;
	return b * sin(t);
}

static real
y(real t, __UNUSED void *p)
{
	return cos(t);
}

static real
dx(real t, void *p)
{
	real b;

	b = *(real *) p;
	return b * cos(t);
}

static real
dy(real t, __UNUSED void *p)
{
	return -sin(t);
}

int
skel_ellipse_ini(int n, real B, /**/ real * plength, real ** pu,
				 real ** pv, T ** pq)
{
	Arc *arc;
	real a, b, t, length, *points, *u, *v;
	int i;

	if (n < 0)
		ERR(CO_INDEX, "n=%d < 0", n);

	MALLOC(n, &u);
	MALLOC(n, &v);

	a = 0;
	b = 2 * PI;
	arc_xy_ini(n, a, b, dx, dy, &B, &arc);
	arc_points(arc, &points);
	arc_length(arc, &length);
	for (i = 0; i < n; i++) {
		t = points[i];
		u[i] = x(t, &B);
		v[i] = y(t, &B);
	}

	skel_close_ini(n, pq);
	*pu = u;
	*pv = v;
	*plength = length;
	arc_fin(arc);
	return CO_OK;
}
