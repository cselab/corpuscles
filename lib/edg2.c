#include <stdio.h>
#include <tgmath.h>
#include "real.h"
#include "co/err.h"
#include "co/vec2.h"
#include "co/edg2.h"

enum
{
	X, Y
};

real
edg2_sq(const real a[2], const real b[2])
{
	real u[2];
	vec2_minus(b, a, u);
	return vec2_dot(u, u);
}

real
edg2_point_distance2(const real a[2], const real b[2], const real p[2])
{
	real t, s, x, y;

	s = edg2_sq (a, b);
	if (s == 0)
		return edg2_sq(p, a);
	t = ((b[X] - a[X])*(p[X] - a[X]) + (b[Y] - a[Y])*(p[Y] - a[Y]))/s;
	if (t > 1.0)
		return edg2_sq(p, b);
	if (t < 0.0)
		return edg2_sq(p, a);
	x = (1 - t)*a[X] + t*b[X] - p[X];
	y = (1 - t)*a[Y] + t*b[Y] - p[Y];
	return x*x + y*y;
}

real
edg2_point_distance(const real a[2], const real b[2], const real p[2])
{
	return sqrt(edg2_point_distance2(a, b, p));
}

int
edg2_point_closest(const real a[2], const real b[2], const real p[2], real q[2])
{
	real t, s, x, y;
	s = edg2_sq (a, b);
	if (s == 0)
		return vec2_copy(a, q);
	t = ((b[X] - a[X])*(p[X] - a[X]) + (b[Y] - a[Y])*(p[Y] - a[Y]))/s;
	if (t > 1.0)
		return vec2_copy(b, q);
	if (t < 0.0)
		return vec2_copy(a, q);
	x = (1 - t)*a[X] + t*b[X];
	y = (1 - t)*a[Y] + t*b[Y];
	return vec2_ini(x, y, q);
}