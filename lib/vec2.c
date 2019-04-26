#include <tgmath.h>
#include <stdio.h>

#include "real.h"
#include "inc/def.h"
#include "co/err.h"
#include "co/vec2.h"

enum
{
	X, Y
};

int
vec2_get(int i, const real *x, const real *y, real a[2])
{
	a[X] = x[i];
	a[Y] = y[i];
	return CO_OK;
}

int
vec2_ini(real x, real y, real a[3])
{
	a[X] = x;
	a[Y] = y;
	return CO_OK;
}

int
vec2_minus(const real a[2], const real b[2], /**/ real c[2])
{
	c[X] = a[X] - b[X];
	c[Y] = a[Y] - b[Y];
	return CO_OK;
}

real
vec2_dot(const real a[2], const real b[2])
{
	return a[X]*b[X] + a[Y]*b[Y];
}

int
vec2_copy(const real a[2], real b[2])
{
	b[X] = a[X];
	b[Y] = a[Y];
	return CO_OK;
}