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
#include <co/kernel.h>

#define FMT CO_REAL_OUT

/*
	r/|r| * dw = z*dwr
	R, Fz(param, r), param
	r, normal, point return Force

*/

#define T PreCons
static const int n = 20;
static const int type =  STEFFEN;
static const real eps = 1e-8;

typedef struct T T;
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
	return z*p->F(r, p->param);
}

int
pre_cons_ini(real R, real (*F)(real, void*), void *param, T  **pq)
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
		MSG(FMT " " FMT, x[i], y[i]);
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
pre_cons_kernel_ini(real R, Kernel *kernel, T  **pq)
{
	Fparam fparam;
	fparam.k = kernel;
	fparam.size = R;
	return pre_cons_ini(R, F, &fparam, pq);
}

int
pre_cons_apply(T *q, real r[3], real point[3], real n[3], /**/ real f[3])
{
	real p[3], d, f0;

	if (fabs(vec_abs(n) - 1) > eps)
		ERR(CO_NUM, "vec_abs(n) != 1");

	vec_minus(r, point, p);
	d = vec_project_scalar(p, n);
	MSG("d: " FMT " " FMT, d, q->R);
	if (d < 0)
		d = 0;
	if (d > q->R)
		f0 = 0;
	else
		f0 = alg_spline_apply(q->s, d);
	MSG("f0: " FMT, f0);
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
	enum {
		X, Y, Z	};
	real size, R;
	Kernel *kernel;
	PreCons *pre_cons;
	real r[3], point[3], norm[3], f[3];

	R = 1;
	kernel_ini(KERNEL_3D, KERNEL_QUINTIC, &kernel);
	pre_cons_kernel_ini(R, kernel, &pre_cons);
	point[X] = point[Y] = point[Z] = 0;
	r[X] = 0.2;
	r[Y] = 0.2;
	r[Z] = 0.2;
	norm[X] = 1;
	norm[Y] = 0;
	norm[Z] = 0;
	pre_cons_apply(pre_cons, r, point, norm, f);
	vec_printf(f, FMT);
	pre_cons_fin(pre_cons);
	kernel_fin(kernel);
}
