#include <tgmath.h>
#include <stdio.h>

#include "real.h"
#include "co/memory.h"
#include "co/err.h"
#include "co/arc.h"

#include <alg/root.h>
#include <alg/integration.h>

#define T Arc
#define FMT CO_REAL_OUT
enum {
	INTEGRATION = GAUSS61
};
enum {
	ROOT = BRENT
};

struct T {
	real *points, length;
};

typedef struct Param Param;
struct Param {
	real(*f) (real, void *);
	void *p;
	AlgIntegration *integ;
	real a, h;
};

static real
F(real x, void *v)
{
	Param *p;
	real r;

	p = v;
	alg_integration_apply(p->integ, p->a, x, p->f, p->p, &r);
	return r - (p->h);
}

int
arc_velocity_ini(int n, real a, real b, real(*f) (real, void *), void *p,
				 T ** pq)
{
	T *q;
	real *points, length, d;
	AlgIntegration *integ;
	AlgRoot *root;
	Param param;
	int i, status;

	if (n < 0)
		ERR(CO_NUM, "n = %d < 0", n);
	if (b < a)
		ERR(CO_NUM, "b=" FMT " < a=" FMT, b, a);

	MALLOC(1, &q);
	MALLOC(n + 1, &points);
	alg_integration_ini(INTEGRATION, &integ);
	status = alg_root_ini(ROOT, &root);
	if (status != CO_OK)
		ERR(CO_INDEX, "alg_root_inim failed");

	alg_integration_apply(integ, a, b, f, p, &length);
	if (length < 0)
		ERR(CO_NUM, "length < 0");

	param.f = f;
	param.p = p;
	param.integ = integ;

	points[0] = a;
	points[n] = b;
	d = length / n;
	for (i = 1; i < n; i++) {
		param.a = points[i - 1];
		param.h = d;
		status =
			alg_root_apply(root, points[i - 1], b, F, &param, &points[i]);
		if (status != CO_OK)
			ERR(CO_INDEX, "alg_root_apply failed: %d %d", i, n);
	}
	alg_root_fin(root);
	alg_integration_fin(integ);
	q->length = length;
	q->points = points;
	*pq = q;
	return CO_OK;
}

typedef struct XYParam XYParam;
struct XYParam {
	real(*x) (real, void *);
	real(*y) (real, void *);
	void *p;
};

static real
XY(real t, void *v)
{
	XYParam *p;
	real x0, y0;

	p = v;

	x0 = p->x(t, p->p);
	y0 = p->y(t, p->p);

	return sqrt(x0 * x0 + y0 * y0);
}

int
arc_xy_ini(int n, real a, real b, real(*x) (real, void *),
		   real(*y) (real, void *), void *p, T ** pq)
{
	XYParam param;

	param.x = x;
	param.y = y;
	param.p = p;
	return arc_velocity_ini(n, a, b, XY, &param, pq);
}

int
arc_fin(T * q)
{
	FREE(q->points);
	FREE(q);
	return CO_OK;
}

int
arc_points(T * q, real ** p)
{
	*p = q->points;
	return CO_OK;
}

int
arc_length(T * q, real * p)
{
	*p = q->length;
	return CO_OK;
}
