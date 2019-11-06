#include <stdio.h>
#include <math.h>
#include <real.h>

#include <co/arc.h>
#include <co/err.h>

#define FMT CO_REAL_OUT
#define PI (3.141592653589793)

static const real A= 0.54353, B = 0.121435, C = -0.561365;
static real
F(real q)
{
	return A*q + B*q*q*q + C*q*q*q*q*q;
}

static real
dF(real q)
{
	return A + 3*B*q*q + 5*C*q*q*q*q;
}

static real
x(real t, void *p)
{
	real alpha;
	alpha = *(real*)p;
	return sin(t);
}

static real
y(real t, void *p)
{
	real alpha;
	alpha = *(real*)p;
	return F(cos(t));
}

static real
dx(real t, void *p)
{
	real alpha;
	alpha = *(real*)p;
	return cos(t);
}

static real
dy(real t, void *p)
{
	real alpha;
	alpha = *(real*)p;
	return -dF(cos(t))*sin(t);
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

