#include <stdio.h>
#include <tgmath.h>

#include "real.h"

#include "co/err.h"
#include "co/integral/sph_plane.h"
#include "co/kernel.h"
#include "co/macro.h"
#include "co/memory.h"
#include "co/vec.h"

#include "co/pre/visc.h"

typedef struct AlgSpline AlgSpline;
enum
{
	LINEAR, POLYNOMIAL, CSPLINE, AKIMA, STEFEN
};
int alg_spline_ini(int n, real*, real*, int type, AlgSpline**);
int alg_spline_fin(AlgSpline*);
real alg_spline_apply(AlgSpline*, real);

#define T PreVisc
#define FMT CO_REAL_OUT

static const int n = 20;
static const int type =  STEFEN;
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
	real beta, R, d;
};

static real
E(real x, real y, real z, void *param)
{
	real beta, R, r, d;
	Eparam *p;
	p = param;

	if (beta < 0 || beta > 1)
		ERR(CO_INDEX, "invalid beta = " FMT);

	beta = p->beta;
	R = p->R;
	r = sqrt(x*x + y*y + z*z);
	d = d/R < beta ? R*beta : d;
	return p->F(r, p->param)*(1 - z/d);
}

int
pre_visc_ini(real R, real beta, real (*F)(real, void*), void *param, T  **pq)
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
	p.beta = beta;
	p.R = R;
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
pre_visc_kernel_ini(real R, real beta, Kernel *kernel, T **pq)
{
	Fparam fparam;
	fparam.k = kernel;
	fparam.size = R;
	return pre_visc_ini(R, beta, F, &fparam, pq);
}

int
pre_visc_apply(T *q, real r[3], real point[3], real n[3], /**/ real *f)
{
	real p[3], d, f0;

	if (fabs(vec_abs(n) - 1) > eps)
		ERR(CO_NUM, "vec_abs(n) != 1");

	vec_minus(r, point, p);
	d = vec_dot(p, n);
	if (d < 0)
		d = 0;
	if (d > q->R)
		*f = 0;
	else
		*f = alg_spline_apply(q->s, d);
	return CO_OK;
}

int
pre_visc_fin(T *q)
{
	alg_spline_fin(q->s);
	FREE(q);
	return CO_OK;
}