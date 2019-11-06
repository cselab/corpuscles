#include <stdio.h>
#include <math.h>
#include <real.h>

#include <co/arc.h>
#include <co/err.h>

#define FMT CO_REAL_OUT
#define PI (3.141592653589793)

static const real A= 1, B = 0.25;
static const int m = 3;

static real
F(real t)
{
	return A + B*cos(m*t);
}

static real
dF(real t)
{
	return -m*B*sin(m*t);
}

static real
x(real t, void *p)
{ 
	return F(t)*cos(t);
}

static real
y(real t, void *p)
{
	return F(t)*sin(t);
}

static real
dx(real t, void *p)
{
	return dF(t)*cos(t) - F(t)*sin(t);
}

static real
dy(real t, void *p)
{
	return dF(t)*sin(t) + F(t)*cos(t);
}

int
main(void)
{
	Arc *q;
	int n, i;
	real alpha, a, b, length, *points, t;

	n = 40;
	a = 0;
	b = 2*PI;
	alpha = 0.75;

	arc_xy_ini(n, a, b, dx, dy, &alpha, &q);
	arc_points(q, &points);
	arc_length(q, &length);

	for (i = 0; i < n + 1; i++) {
		t = points[i];
		printf(FMT " " FMT " " FMT "\n", t, x(t, &alpha), y(t, &alpha));
	}

	MSG("length " FMT, length);
	arc_fin(q);
}
