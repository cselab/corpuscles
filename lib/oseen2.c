#include <tgmath.h>
#include <stdio.h>
#include "real.h"
#include "co/err.h"
#include "co/macro.h"
#include "co/matrix.h"
#include "co/memory.h"
#include "co/oseen2.h"
#include "co/skel.h"
#include "co/vec2.h"
#include "co/sum.h"

#define T Oseen2
static const real pi = 3.141592653589793115997964;
struct T
{
	real e;
};

static int
oseen(__UNUSED real e, const real a[2], const real b[2], real *xx, real *xy, real *yy)
{
	enum
	{
		X, Y
	};
	real d[2], r, r2,l;

	vec2_minus(a, b, d);
	r = vec2_abs(d);
	if (r == 0)
		ERR(CO_NUM, "r == 0");
	r2 = r*r;
	l = log(r);
	*xx = -l + d[X]*d[X]/r2;
	*xy = d[X]*d[Y]/r2;
	*yy = -l + d[Y]*d[Y]/r2;
	return CO_OK;
}

static int
oseen0(real e, real *xx, real *xy, real *yy)
{
	real l;
	l = log(2*e) - 3/2;
	*xx = -l;
	*xy = 0;
	*yy = -l;
	return CO_OK;
}

int
oseen2_ini(real e, T **pq)
{
	T *q;
	MALLOC(1, &q);
	q->e = e;
	*pq = q;
	return CO_OK;
}

int
oseen2_fin(T *q)
{
	FREE(q);
	return CO_OK;
}

int
oseen2_apply(T *q, Skel *skel, const real *x, const real *y, real *oxx, real *oxy, real *oyy)
{
	int n, i, j;
	real e, s, a[2], b[2], xx, xy, yy;
	n = skel_nv(skel);
	e = q->e;

	for (i = 0; i < n; i++) {
		vec2_get(i, x, y, a);
		oseen0(e, &xx, &xy, &yy);
		matrix_set(n, n, i, i, xx, oxx);
		matrix_set(n, n, i, i, xy, oxy);
		matrix_set(n, n, i, i, yy, oyy);
		for (j = 0; j < n; j++) {
			if (i == j) continue;
			vec2_get(j, x, y, b);
			oseen(e, a, b, &xx, &xy, &yy);
			matrix_set(n, n, i, j, xx, oxx);
			matrix_set(n, n, i, j, xy, oxy);
			matrix_set(n, n, i, j, yy, oyy);
		}
	}
	s = 1/(4*pi);
	matrix_scale(n, n, s, oxx);
	matrix_scale(n, n, s, oxy);
	matrix_scale(n, n, s, oyy);
	return CO_OK;
}

real
oseen2_pressure(T *q, Skel *skel, const real *x, const real *y, const real *fx, const real *fy, real u, real v)
{
	int n, i;
	real p, s, r[2], a[2], f[2], d[2];
	HeSum *sum;

	n = skel_nv(skel);
	he_sum_ini(&sum);
	vec2_ini(u, v, r);
	for (i = 0; i < n; i++) {
		vec2_get(i, x, y, a);
		vec2_get(i, fx, fy, f);
		vec2_minus(r, a, d);
		he_sum_add(sum, vec2_dot(f, d)/vec2_dot(d, d));
	}
	p = he_sum_get(sum);
	he_sum_fin(sum);
	s = 1/(2*pi);
	return s*p;
}
