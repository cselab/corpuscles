#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>

#include <real.h>
#include <co/arg.h>
#include <co/err.h>
#include <co/memory.h>
#include <co/macro.h>
#include <co/vec.h>
#include <co/integral/sph_plane.h>
#include <alg/spline.h>

#define FMT CO_REAL_OUT


/*
	r/|r| * dw = z*dwr
	R, Fz(param, r), param
	r, normal, point return Force

*/

#define T PreCons
static const int n = 100;
static const int type =  STEFFEN;
static const real eps = 1e-8;

typedef struct T T;
struct T
{
	AlgSpline *s;
};

typedef struct Eparam Eparam;
struct Eparam
{
	real (*F)(real, void*);
	void *param;
};

static real
E(real x, real y, real z, void *param)
{
	real r;
	Eparam *p;
	p = param;
	r = sqrt(x*x + y*y + z*z);
	return z*p->F(r, p->param);
}

int
pre_cons_ini(real R, real (*F)(real, void*), void *param, T  **pq)
{
	T *q;
	real *x, *y, res, d;
	int i;
	Eparam p;

	MALLOC(1, &q);
	SphPlane *integ;
	sph_plane_ini(R, &integ);
	MALLOC(n, &x);
	MALLOC(n, &y);
	p.F = F;
	p.param = param;
	for (i = 0; i < n; i++) {
		d = R/(n - 1)*i;
		sph_plane_apply(integ, d, E, &p, &res);
		x[i] = d;
		y[i] = res;
	}
	alg_spline_ini(n, x, y, type, &q->s);
	sph_plane_fin(integ);
	FREE(x);
	FREE(y);
	*pq = q;
	return CO_OK;
}

int
pre_cons_apply(T *q, real r[3], real point[3], real n[3], /**/ real f[3])
{
	real p[3], d, f0;

	if (fabs(vec_abs(n) - 1) > eps)
		ERR(CO_NUM, "vec_abs(n) != 1");

	vec_minus(r, point, p);
	d = vec_project_scalar(p, n);
	if (d < 0)
		d = 0;
	f0 = alg_spline_apply(q->s, d);
	vec_scalar(n, f0, f);
	return CO_OK;
}

int
pre_cons_fin(T *q)
{
	alg_spline_fin(q->s);
	FREE(q);
	return CO_OK;
}

int
main(void)
{

}
