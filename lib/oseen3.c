#include <tgmath.h>
#include <stdio.h>
#ifdef _OPENMP
#include <omp.h>
#endif
#include "real.h"
#include "co/err.h"
#include "co/he.h"
#include "co/macro.h"
#include "co/matrix.h"
#include "co/memory.h"
#include "co/oseen3.h"
#include "co/vec.h"

#define T Oseen3
static const real pi = 3.141592653589793115997964;
struct T
{
	real e;
};

static int
oseen(__UNUSED real e, const real a[3], const real b[3],
	real *xx, real *xy, real *xz, real *yy, real *yz, real *zz)
{
	enum
	{
		X, Y, Z
	};
	real d[3], r, r3,l;

	vec_minus(a, b, d);
	r = vec_abs(d);
	if (r == 0)
		ERR(CO_NUM, "r == 0");
	r3 = r*r*r;
	l = 1/r;
	*xx = l + d[X]*d[X]/r3;
	*yy = l + d[Y]*d[Y]/r3;
	*zz = l + d[Z]*d[Z]/r3;

	*xy = d[X]*d[Y]/r3;
	*xz = d[X]*d[Z]/r3;
	*yz = d[Y]*d[Z]/r3;
	return CO_OK;
}

static int
oseen0(real e, real *xx, real *xy, real *xz, real *yy, real *yz, real *zz)
{
	real l;

	l = 2/e;
	*xx = *yy = *zz = l;
	*xy = *xz = *yz = 0;
	return CO_OK;
}

int
oseen3_ini(real e, T **pq)
{
	T *q;

	MALLOC(1, &q);
	q->e = e;
	*pq = q;
	return CO_OK;
}

int
oseen3_fin(T *q)
{
	FREE(q);
	return CO_OK;
}

int
oseen3_apply(T *q, He *he, const real *x, const real *y, const real *z,
	real *oxx, real *oxy, real *oxz, real *oyy, real *oyz, real *ozz)
{
	int n, i, j;
	real e, s, a[3], b[3], xx, xy, xz, yy, yz, zz;

	n = he_nv(he);
	e = q->e;
#pragma omp parallel for
	for (i = 0; i < n; i++) {
		vec_get(i, x, y, z, a);
		oseen0(e, &xx, &xy, &xz, &yy, &yz, &zz);
		matrix_set(n, n, i, i, xx, oxx);
		matrix_set(n, n, i, i, xy, oxy);
		matrix_set(n, n, i, i, xz, oxz);
		matrix_set(n, n, i, i, yy, oyy);
		matrix_set(n, n, i, i, yz, oyz);
		matrix_set(n, n, i, i, zz, ozz);
		for (j = i+1; j < n; j++) {
			vec_get(j, x, y, z, b);
			if (oseen(e, a, b, &xx, &xy, &xz, &yy, &yz, &zz) != CO_OK)
				ERR(CO_INDEX, "ij %d %d", i, j);
			matrix_set(n, n, i, j, xx, oxx);
			matrix_set(n, n, i, j, xy, oxy);
			matrix_set(n, n, i, j, xz, oxz);
			matrix_set(n, n, i, j, yy, oyy);
			matrix_set(n, n, i, j, yz, oyz);
			matrix_set(n, n, i, j, zz, ozz);

			matrix_set(n, n, j, i, xx, oxx);
			matrix_set(n, n, j, i, xy, oxy);
			matrix_set(n, n, j, i, xz, oxz);
			matrix_set(n, n, j, i, yy, oyy);
			matrix_set(n, n, j, i, yz, oyz);
			matrix_set(n, n, j, i, zz, ozz);
		}
	}
	s = 1/(8*pi);
	matrix_scale(n, n, s, oxx);
	matrix_scale(n, n, s, oxy);
	matrix_scale(n, n, s, oxz);
	matrix_scale(n, n, s, oyy);
	matrix_scale(n, n, s, oyz);
	matrix_scale(n, n, s, ozz);
	return CO_OK;
}
