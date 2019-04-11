#include <stdio.h>
#include <tgmath.h>

#include "real.h"

#include "co/err.h"
#include "co/integral/sph_plane.h"
#include "co/kernel.h"
#include "co/macro.h"
#include "co/memory.h"
#include "co/vec.h"

#include "co/pre/density.h"

typedef struct AlgSpline AlgSpline;
enum {LINEAR, POLYNOMIAL, CSPLINE, AKIMA, STEFFEN};
int alg_spline_ini(int n, real*, real*, int type, AlgSpline**);
int alg_spline_fin(AlgSpline*);
real alg_spline_apply(AlgSpline*, real);

#define T PreDensity

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
E(real x, real y, real z, void *param)
{
	real r;
	Eparam *p;
	p = param;
	r = sqrt(x*x + y*y + z*z);
	return p->F(r, p->param);
}

int
pre_density_ini(real R, real (*F)(real, void*), void *param, T  **pq)
{
	T *q;
	real *x, *y, res, volume, d;
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
		sph_plane_volume(integ, d, &volume);
		x[i] = d;
		if (volume > eps)
			y[i] = res/volume;
		else
			y[i] = 0;
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
	return kernel_w(p->k, p->size, r);
}

int
pre_density_kernel_ini(real R, Kernel *kernel, T  **pq)
{
	Fparam fparam;
	fparam.k = kernel;
	fparam.size = R;
	return pre_density_ini(R, F, &fparam, pq);
}

int
pre_density_apply(T *q, real r[3], real point[3], real n[3], /**/ real f[3])
{
	real p[3], d, f0;

	if (fabs(vec_abs(n) - 1) > eps)
		ERR(CO_NUM, "vec_abs(n) != 1");

	vec_minus(r, point, p);
	d = vec_project_scalar(p, n);
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
pre_density_fin(T *q)
{
	alg_spline_fin(q->s);
	FREE(q);
	return CO_OK;
}