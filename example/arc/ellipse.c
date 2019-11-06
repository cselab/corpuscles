#include <stdio.h>
#include <math.h>
#include <real.h>

#include <co/arc.h>
#include <co/err.h>

#define FMT CO_REAL_OUT
#define PI (3.141592653589793)
static real
x(real t, void *p)
{
	real alpha;
	alpha = *(real*)p;
	return cos(t);
}

static real
y(real t, void *p)
{
	real alpha;
	alpha = *(real*)p;
	return alpha*sin(t);
}

static real
dx(real t, void *p)
{
	real alpha;
	alpha = *(real*)p;
	return -sin(t);
}

static real
dy(real t, void *p)
{
	real alpha;
	alpha = *(real*)p;
	return alpha*cos(t);
}

int
main(void)
{
	Arc *q;
	int n, i;
	real alpha, a, b, length, *points, t;

	n = 20;
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

