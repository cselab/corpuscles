#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>

#include <real.h>
#include <co/arg.h>
#include <co/err.h>
#include <co/memory.h>
#include <co/integral/sph_plane.h>

#include <alg/spline.h>

#define FMT CO_REAL_OUT


/*
	r/|r| * dw = z*dwr
	R, Fz(param, r), param
	r, normal, point return Force

*/

#define T PreCons
const int n = 100;
const int type =  STEFFEN;

typedef struct T T;
struct T
{
	AlgSpline *s;
};

static real
E(real x, real y, real z, void *p)
{
	real r;
	return r;
}

int
pre_cons_ini(real R, real (*F)(void*, real), void *param, T  **pq)
{
	T *q;
	real *x, *y, res, d;
	int i;

	MALLOC(1, &q);
	SphPlane *integ;
	sph_plane_ini(R, &integ);
	MALLOC(n, &x);
	MALLOC(n, &y);
	alg_spline_ini(n, x, y, type, &q->s);
	for (i = 0; i < n; i++) {
		d = R/(n - 1)*i;
		sph_plane_apply(integ, d, E, NULL, &res);
		x[i] = d;
		y[i] = res;
	}
	sph_plane_fin(integ);
	FREE(x);
	FREE(y);

	*pq = q;
	return CO_OK;
}

int
pre_cons_fin(T *q)
{
	alg_spline_fin(q->s);
	FREE(q);
}

int
main(int argc, char **argv)
{

}
