#include <stdio.h>
#include <tgmath.h>
#include "real.h"
#include "co/err.h"
#include "co/vec.h"
#include "co/vec2.h"
#include "co/edg2.h"

#define FMT CO_REAL_OUT

enum {
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
edg2_abs(const real a[2], const real b[2])
{
	real u[2];

	vec2_minus(b, a, u);
	return vec2_abs(u);
}

#define NOT_ZERO(x) if ((x) == 0) ERR(CO_NUM, "should not be zero")
real
edg2_alpha(const real a[2], const real b[2], const real u[2],
		   const real v[2])
{
	real A, B;

	A = edg2_abs(a, b);
	B = edg2_abs(u, v);
	NOT_ZERO(A);
	return B / A - 1;
}

real
edg2_strain(real Ka, real a3, real a4, const real a[2], const real b[2],
			const real u[2], const real v[2])
{
	real l;

	l = edg2_alpha(a, b, u, v);
	return Ka / 2 * (l * l + a3 * l * l * l + a4 * l * l * l * l);
}

real
edg2_point_distance2(const real a[2], const real b[2], const real p[2])
{
	real t, s, x, y;

	s = edg2_sq(a, b);
	if (s == 0)
		return edg2_sq(p, a);
	t = ((b[X] - a[X]) * (p[X] - a[X]) +
		 (b[Y] - a[Y]) * (p[Y] - a[Y])) / s;
	if (t > 1.0)
		return edg2_sq(p, b);
	if (t < 0.0)
		return edg2_sq(p, a);
	x = (1 - t) * a[X] + t * b[X] - p[X];
	y = (1 - t) * a[Y] + t * b[Y] - p[Y];
	return x * x + y * y;
}

real
edg2_point_distance(const real a[2], const real b[2], const real p[2])
{
	return sqrt(edg2_point_distance2(a, b, p));
}

int
edg2_point_closest(const real a[2], const real b[2], const real p[2],
				   real q[2])
{
	real t, s, x, y;

	s = edg2_sq(a, b);
	if (s == 0)
		return vec2_copy(a, q);
	t = ((b[X] - a[X]) * (p[X] - a[X]) +
		 (b[Y] - a[Y]) * (p[Y] - a[Y])) / s;
	if (t > 1.0)
		return vec2_copy(b, q);
	if (t < 0.0)
		return vec2_copy(a, q);
	x = (1 - t) * a[X] + t * b[X];
	y = (1 - t) * a[Y] + t * b[Y];
	return vec2_ini(x, y, q);
}

int
edg2_vect(const real a[2], const real b[2], FILE * f)
{
	real zero = 0;
	int status;

	status = fputs("VECT\n" "1 2 0\n" "2\n" "0\n", f);
	if (status == EOF)
		ERR(CO_IO, "fail to write");
	fprintf(f, FMT "  " FMT " " FMT "\n", a[X], a[Y], zero);
	fprintf(f, FMT "  " FMT " " FMT "\n", b[X], b[Y], zero);
	return CO_OK;
}
