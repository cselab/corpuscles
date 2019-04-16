#include <stdio.h>
#include <tgmath.h>

#include "real.h"

#include "co/err.h"
#include "co/integral/sph_plane.h"
#include "co/integral/circle_line.h"
#include "co/kernel.h"
#include "co/macro.h"
#include "co/memory.h"
#include "co/vec.h"

#include "co/pre/cons.h"

typedef struct AlgSpline AlgSpline;
enum {LINEAR, POLYNOMIAL, CSPLINE, AKIMA, STEFFEN};
int alg_spline_ini(int n, real*, real*, int type, AlgSpline**);
int alg_spline_fin(AlgSpline*);
real alg_spline_apply(AlgSpline*, real);

#define T PreCons

static const int n = 20;
static const int type =  STEFFEN;
static const real eps = 1e-8;

struct T
{
	real R;
	AlgSpline *s;
};

typedef struct Eparam Eparam;
struct Eparam
{
	real (*F)(real, void*);
	void *param;
};

static real
E3(real x, real y, real z, void *param)
{
	real r;
	Eparam *p;
	p = param;
	r = sqrt(x*x + y*y + z*z);
	return z*p->F(r, p->param);
}

static real
E2(real x, real y, void *param)
{
	real r;
	Eparam *p;
	p = param;
	r = sqrt(x*x + y*y);
	return y*p->F(r, p->param);
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
		sph_plane_apply(integ, d, E3, &p, &res);
		x[i] = d;
		y[i] = res;
	}
	alg_spline_ini(n, x, y, type, &q->s);
	sph_plane_fin(integ);
	FREE(x);
	FREE(y);

	q->R = R;
	*pq = q;
	return CO_OK;
}

int
pre_cons2_ini(real R, real (*F)(real, void*), void *param, T  **pq)
{
	T *q;
	real *x, *y, res, d;
	int i;
	Eparam p;

	MALLOC(1, &q);
	CircleLine *integ;
	circle_line_ini(R, &integ);
	MALLOC(n, &x);
	MALLOC(n, &y);
	p.F = F;
	p.param = param;
	for (i = 0; i < n; i++) {
		d = R/(n - 1)*i;
		circle_line_apply(integ, d, E2, &p, &res);
		x[i] = d;
		y[i] = res;
	}
	alg_spline_ini(n, x, y, type, &q->s);
	circle_line_fin(integ);
	FREE(x);
	FREE(y);

	q->R = R;
	*pq = q;
	return CO_OK;
}

typedef struct Fparam Fparam;
struct Fparam
{
	Kernel *k;
	real size;
};

static real
F(real r, void *param)
{
	Fparam *p;
	p = param;
	return kernel_dwr(p->k, p->size, r);
}

int
pre_cons_kernel_ini(real R, Kernel *kernel, T  **pq)
{
	Fparam fparam;
	fparam.k = kernel;
	fparam.size = R;
	return pre_cons_ini(R, F, &fparam, pq);
}

int
pre_cons2_kernel_ini(real R, Kernel *kernel, T  **pq)
{
	Fparam fparam;
	fparam.k = kernel;
	fparam.size = R;
	return pre_cons2_ini(R, F, &fparam, pq);
}

int
pre_cons_apply(T *q, real r[3], real point[3], real n[3], /**/ real f[3])
{
	real p[3], d, f0;

	if (fabs(vec_abs(n) - 1) > eps)
		ERR(CO_NUM, "vec_abs(n) != 1");

	vec_minus(r, point, p);
	d = vec_dot(p, n);
	if (d < 0)
		d = 0;
	if (d > q->R)
		f0 = 0;
	else
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