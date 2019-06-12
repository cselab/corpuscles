#include <tgmath.h>
#include <stdio.h>
#include <omp.h>
#include "real.h"
#include "co/err.h"
#include "co/he.h"
#include "co/macro.h"
#include "co/matrix.h"
#include "co/memory.h"
#include "co/normal.h"
#include "co/oseen3.h"
#include "co/vec.h"

#define T Oseen3
static const real pi = 3.141592653589793115997964;
struct T
{
	real e;
	real *nx, *ny, *nz;
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
oseen3_he_ini(He *he, real e, T **pq)
{
	T *q;
	real *nx, *ny, *nz;
	int n;

	MALLOC(1, &q);
	n = he_nv(he);
	MALLOC3(n, &nx, &ny, &nz);
	q->e = e;
	q->nx = nx;
	q->ny = ny;
	q->nz = nz;
	*pq = q;
	return CO_OK;
}

int
oseen3_fin(T *q)
{
	FREE(q);
	//FREE3(q->nx, q->ny, q->nz);
	return CO_OK;
}

#define SET(i, j, s, a) matrix_set(n, n, i, j, s, a)

int
oseen3_apply(T *q, He *he, const real *x, const real *y, const real *z,
	real *oxx, real *oxy, real *oxz, real *oyy, real *oyz, real *ozz)
{
	int n, i;
	real e, s;

	n = he_nv(he);
	e = q->e;
#pragma omp parallel for
	for (i = 0; i < n; i++) {
		real a[3], b[3], xx, xy, xz, yy, yz, zz;
		int j;
		vec_get(i, x, y, z, a);
		oseen0(e, &xx, &xy, &xz, &yy, &yz, &zz);
		SET(i, i, xx, oxx);
		SET(i, i, xy, oxy);
		SET(i, i, xz, oxz);
		SET(i, i, yy, oyy);
		SET(i, i, yz, oyz);
		SET(i, i, zz, ozz);
		for (j = i + 1; j < n; j++) {
			vec_get(j, x, y, z, b);
			oseen(e, a, b, &xx, &xy, &xz, &yy, &yz, &zz);
			SET(i, j, xx, oxx);
			SET(i, j, xy, oxy);
			SET(i, j, xz, oxz);
			SET(i, j, yy, oyy);
			SET(i, j, yz, oyz);
			SET(i, j, zz, ozz);

			SET(j, i, xx, oxx);
			SET(j, i, xy, oxy);
			SET(j, i, xz, oxz);
			SET(j, i, yy, oyy);
			SET(j, i, yz, oyz);
			SET(j, i, zz, ozz);
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

static int
stresslet(__UNUSED real e, const real a[3], const real n[3], const real b[3],
	real *xx, real *xy, real *xz, real *yy, real *yz, real *zz)
{
	enum
	{
		X, Y, Z
	};
	real d[3], r, r5, p, l;

	vec_minus(a, b, d);
	r = vec_abs(d);
	p = vec_dot(d, n);
	if (r == 0)
		ERR(CO_NUM, "r == 0");
	r5= r*r*r*r*r;
	l = p/r5;
	*xx = d[X]*d[X]*l;
	*xy = d[X]*d[Y]*l;
	*xz = d[X]*d[Z]*l;
	*yy = d[Y]*d[Y]*l;
	*yz = d[Y]*d[Z]*l;
	*zz = d[Z]*d[Z]*l;
	return CO_OK;
}

static int
stresslet0(real e, const real a[3], const real n[3],
	real *xx, real *xy, real *xz, real *yy, real *yz, real *zz)
{
	real l;

	l = 2/e;
	*xx = *yy = *zz = l;
	*xy = *xz = *yz = 0;
	return CO_OK;
}

int
oseeen3_stresslet(T *q, He *he, const real*x, const real *y, const real *z,
	real *oxx, real *oxy, real *oxz, real *oyy, real *oyz, real *ozz)
{
	real *nx, *ny, *nz, s, e;
	int status, n, i;

	e = q->e;
	nx = q->nx;
	ny = q->ny;
	nz = q->nz;
	status = normal_mwa(he, x, y, z, nx, ny, nz);
	if (status != CO_OK)
		ERR(CO_NUM, "normal_mwa failed");
	n = he_nv(he);
	for (i = 0; i < n; i++) {
		real a[3], b[3], u[3], xx, xy, xz, yy, yz, zz;
		int j;
		vec_get(i, x, y, z, a);
		vec_get(i, nx, ny, nz, u);
		stresslet0(e, a, u, &xx, &xy, &xz, &yy, &yz, &zz);
		SET(i, i, xx, oxx);
		SET(i, i, xy, oxy);
		SET(i, i, xz, oxz);
		SET(i, i, yy, oyy);
		SET(i, i, yz, oyz);
		SET(i, i, zz, ozz);
		for (j = 0; j < n; j++) {
			vec_get(j, x, y, z, b);
			stresslet(e, a, u, b, &xx, &xy, &xz, &yy, &yz, &zz);
			SET(i, j, xx, oxx);
			SET(i, j, xy, oxy);
			SET(i, j, xz, oxz);
			SET(i, j, yy, oyy);
			SET(i, j, yz, oyz);
			SET(i, j, zz, ozz);			
		}
	}
	s = -6/(8*pi);
	matrix_scale(n, n, s, oxx);
	matrix_scale(n, n, s, oxy);
	matrix_scale(n, n, s, oxz);
	matrix_scale(n, n, s, oyy);
	matrix_scale(n, n, s, oyz);
	matrix_scale(n, n, s, ozz);
	return CO_OK;
}
