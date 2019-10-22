#include <stdio.h>
#include <tgmath.h>

#include "real.h"
#include "co/err.h"
#include "co/vec.h"

#include "co/edg.h"

#define FMT CO_REAL_OUT

enum {
	X, Y, Z
};

real
edg_abs(const real a[3], const real b[3])
{
	real u[3];

	vec_minus(b, a, u);
	return vec_abs(u);
}

real
edg_sq(const real a[3], const real b[3])
{
	real u[3];

	vec_minus(b, a, u);
	return vec_dot(u, u);
}

real
edg_point_distance2(const real a[3], const real b[3], const real p[3])
{
	real t, s, x, y, z;

	s = edg_sq(a, b);
	if (s == 0)
		return edg_sq(p, a);
	t = ((b[X] - a[X]) * (p[X] - a[X]) + (b[Y] - a[Y]) * (p[Y] - a[Y]) +
		 (b[Z] - a[Z]) * (p[Z] - a[Z])) / s;
	if (t > 1.0)
		return edg_sq(p, b);
	if (t < 0.0)
		return edg_sq(p, a);
	x = (1 - t) * a[X] + t * b[X] - p[X];
	y = (1 - t) * a[Y] + t * b[Y] - p[Y];
	z = (1 - t) * a[Z] + t * b[Z] - p[Z];
	return x * x + y * y + z * z;
}

real
edg_point_distance(const real a[3], const real b[3], const real p[3])
{
	return sqrt(edg_point_distance2(a, b, p));
}

int
edg_point_closest(const real a[3], const real b[3], const real p[3],
				  real q[3])
{
	real t, s, x, y, z;

	s = edg_sq(a, b);
	if (s == 0)
		return vec_copy(a, q);
	t = ((b[X] - a[X]) * (p[X] - a[X]) + (b[Y] - a[Y]) * (p[Y] - a[Y]) +
		 (b[Z] - a[Z]) * (p[Z] - a[Z])) / s;
	if (t > 1.0)
		return vec_copy(b, q);
	if (t < 0.0)
		return vec_copy(a, q);
	x = (1 - t) * a[X] + t * b[X];
	y = (1 - t) * a[Y] + t * b[Y];
	z = (1 - t) * a[Z] + t * b[Z];
	return vec_ini(x, y, z, q);
}

int
edg_vect(const real a[3], const real b[3], FILE * f)
{
	int status;

	status = fputs("VECT\n" "1 2 0\n" "2\n" "0\n", f);
	if (status == EOF)
		ERR(CO_IO, "fail to write");
	vec_fprintf(a, f, FMT);
	vec_fprintf(b, f, FMT);
	return CO_OK;
}

int
edg_center(const real a[3], const real b[3], real c[3])
{
	c[X] = (a[X] + b[X]) / 2;
	c[Y] = (a[Y] + b[Y]) / 2;
	c[Z] = (a[Z] + b[Z]) / 2;
	return CO_OK;
}
