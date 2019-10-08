#include <tgmath.h>
#include <stdio.h>
#include <omp.h>
#include "real.h"
#include "co/err.h"
#include "co/he.h"
#include "co/macro.h"
#include "co/memory.h"
#include "co/normal.h"
#include "co/oseen3zero.h"
#include "co/i/matrix.h"
#include "co/i/vec.h"
#include "co/i/tri.h"
#include "co/i/area.h"

#define T Oseen3Zero
static const real pi = 3.141592653589793115997964;
struct T
{
	real e;
	real *nx, *ny, *nz, *area;
};

static int
oseen(real e0, const real a[3], const real b[3],
      real *xx, real *xy, real *xz, real *yy, real *yz, real *zz)
{
        USED(e0);
	enum
	{
		X, Y, Z
	};
	real d[3], r, r3, l;

	i_vec_minus(a, b, d);
	r = i_vec_abs(d);
	r3 = r*r*r;
	l = 1/r;
	if (r == 0)
		ERR(CO_NUM, "r == 0");
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
oseen3_zero_ini(He *he, real e, T **pq)
{
	T *q;
	real *nx, *ny, *nz, *area;
	int n;

	MALLOC(1, &q);
	n = he_nv(he);
	MALLOC3(n, &nx, &ny, &nz);
	MALLOC(n, &area);
	q->e = e;
	q->nx = nx;
	q->ny = ny;
	q->nz = nz;
	q->area = area;
	*pq = q;
	return CO_OK;
}

int
oseen3_zero_fin(T *q)
{
	FREE3(q->nx, q->ny, q->nz);
	FREE(q->area);
	FREE(q);
	return CO_OK;
}

#define SET(i, j, s, a) i_matrix_set(n, n, i, j, s, a)

int
oseen3_zero_apply(T *q, He *he, const real *x, const real *y, const real *z,
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
		i_vec_get(i, x, y, z, a);
		oseen0(e, &xx, &xy, &xz, &yy, &yz, &zz);
		SET(i, i, xx, oxx);
		SET(i, i, xy, oxy);
		SET(i, i, xz, oxz);
		SET(i, i, yy, oyy);
		SET(i, i, yz, oyz);
		SET(i, i, zz, ozz);
		for (j = i + 1; j < n; j++) {
			i_vec_get(j, x, y, z, b);
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
	i_matrix_scale(n, n, s, oxx);
	i_matrix_scale(n, n, s, oxy);
	i_matrix_scale(n, n, s, oxz);
	i_matrix_scale(n, n, s, oyy);
	i_matrix_scale(n, n, s, oyz);
	i_matrix_scale(n, n, s, ozz);
	return CO_OK;
}

static int
stresslet(real e0, const real a[3], const real n[3], const real b[3],
	real *xx, real *xy, real *xz, real *yy, real *yz, real *zz)
{
        USED(e0);
	enum
	{
		X, Y, Z
	};
	real d[3], r, r5, p, l6;

	i_vec_minus(a, b, d);
	r = i_vec_abs(d);
	p = i_vec_dot(d, n);
	if (r == 0)
		ERR(CO_NUM, "r == 0");
	r5 = r*r*r*r*r;
	l6 = 6*p/r5;

	*xx = d[X]*d[X]*l6;
	*xy = d[X]*d[Y]*l6;
	*xz = d[X]*d[Z]*l6;
	*yy = d[Y]*d[Y]*l6;
	*yz = d[Y]*d[Z]*l6;
	*zz = d[Z]*d[Z]*l6;
	
	return CO_OK;
}
static int
stresslet0(__UNUSED real e, __UNUSED const real a[3], __UNUSED const real n[3],
	real *xx, real *xy, real *xz, real *yy, real *yz, real *zz)
{
	*xx = *yy = *zz = *xy = *xz = *yz = 0;
	return CO_OK;
}
int
oseen3_zero_stresslet(T *q, He *he, const real *x, const real *y, const real *z,
	real *oxx, real *oxy, real *oxz, real *oyy, real *oyz, real *ozz)
{
	real *nx, *ny, *nz, *area, A, s, e;
	int status, n, i;

	e = q->e;
	nx = q->nx;
	ny = q->ny;
	nz = q->nz;
	area = q->area;
	status = normal_mwa(he, x, y, z, nx, ny, nz);
	if (status != CO_OK)
		ERR(CO_NUM, "normal_mwa failed");
	status = i_area_ver(he, x, y, z, area);
	if (status != CO_OK)
		ERR(CO_NUM, "area_ver failed");
	n = he_nv(he);
#pragma omp parallel for
	for (i = 0; i < n; i++) {
		real a[3], b[3], u[3], xx, xy, xz, yy, yz, zz;
		int j;
		i_vec_get(i, x, y, z, a);
		i_vec_get(i, nx, ny, nz, u);
		A = area[i];
		stresslet0(e, a, u, &xx, &xy, &xz, &yy, &yz, &zz);
		SET(i, i, A*xx, oxx);
		SET(i, i, A*xy, oxy);
		SET(i, i, A*xz, oxz);
		SET(i, i, A*yy, oyy);
		SET(i, i, A*yz, oyz);
		SET(i, i, A*zz, ozz);
		for (j = 0; j < n; j++) {
			if (i == j) continue;
			i_vec_get(j, nx, ny, nz, u);
			i_vec_get(j, x, y, z, b);
			A = area[j];
			stresslet(e, a, u, b, &xx, &xy, &xz, &yy, &yz, &zz);
			SET(i, j, A*xx, oxx);
			SET(i, j, A*xy, oxy);
			SET(i, j, A*xz, oxz);
			SET(i, j, A*yy, oyy);
			SET(i, j, A*yz, oyz);
			SET(i, j, A*zz, ozz);
		}
	}
	s = 1/(8*pi);
	i_matrix_scale(n, n, s, oxx);
	i_matrix_scale(n, n, s, oxy);
	i_matrix_scale(n, n, s, oxz);
	i_matrix_scale(n, n, s, oyy);
	i_matrix_scale(n, n, s, oyz);
	i_matrix_scale(n, n, s, ozz);
	return CO_OK;
}
