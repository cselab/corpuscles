#include <tgmath.h>
#include <stdio.h>
#ifdef _OPENMP
#include <omp.h>
#endif
#include "real.h"
#include "co/err.h"
#include "co/he.h"
#include "co/macro.h"
#include "co/memory.h"
#include "co/normal.h"
#include "co/oseen3tri.h"
#include "co/matrix.h"
#include "co/i/matrix.h"
#include "co/i/vec.h"
#include "co/i/tri.h"
#include "co/tri.h"
#include "co/i/area.h"

#define T Oseen3Tri
static const real pi = 3.141592653589793115997964;
struct T
{
	real *nx, *ny, *nz, *area;
};

static int
oseen(const real a[3], const real b[3],
      real *xx, real *xy, real *xz, real *yy, real *yz, real *zz)
{
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

int
oseen3_tri_ini(He *he, T **pq)
{
	T *q;
	real *nx, *ny, *nz, *area;
	int n;

	MALLOC(1, &q);
	n = he_nv(he);
	MALLOC3(n, &nx, &ny, &nz);
	MALLOC(n, &area);
	q->nx = nx;
	q->ny = ny;
	q->nz = nz;
	q->area = area;
	*pq = q;
	return CO_OK;
}

int
oseen3_tri_fin(T *q)
{
	FREE3(q->nx, q->ny, q->nz);
	FREE(q->area);
	FREE(q);
	return CO_OK;
}

#define SET(i, j, s, a) i_matrix_set(n, n, i, j, s, a)
int
oseen3_tri_apply(T *q, He *he, const real *x, const real *y, const real *z,
	real *oxx, real *oxy, real *oxz, real *oyy, real *oyz, real *ozz)
{
	int n, i;
	real e, s;

	n = he_nv(he);
#pragma omp parallel for
	for (i = 0; i < n; i++) {
		real a[3], b[3], xx, xy, xz, yy, yz, zz;
		int j;
		i_vec_get(i, x, y, z, a);
		SET(i, i, 0, oxx);
		SET(i, i, 0, oxy);
		SET(i, i, 0, oxz);
		SET(i, i, 0, oyy);
		SET(i, i, 0, oyz);
		SET(i, i, 0, ozz);
		for (j = i + 1; j < n; j++) {
			i_vec_get(j, x, y, z, b);
			oseen(a, b, &xx, &xy, &xz, &yy, &yz, &zz);
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
stresslet(const real a[3], const real n[3], const real b[3],
	real *xx, real *xy, real *xz, real *yy, real *yz, real *zz)
{
	enum
	{
		X, Y, Z
	};
	real d[3], r, p, l;

	i_vec_minus(a, b, d);
	r = i_vec_abs(d);
	p = i_vec_dot(d, n);
	if (r == 0)
		ERR(CO_NUM, "r == 0");
	l = p/(r*r*r*r*r);
	*xx = d[X]*d[X]*l;
	*xy = d[X]*d[Y]*l;
	*xz = d[X]*d[Z]*l;
	*yy = d[Y]*d[Y]*l;
	*yz = d[Y]*d[Z]*l;
	*zz = d[Z]*d[Z]*l;

	return CO_OK;
}

#define TSET(i, s, a)							\
    do {								\
	i_matrix_add(nv, nv, i, ia, (s), (a));				\
	i_matrix_add(nv, nv, i, ib, (s), (a));				\
	i_matrix_add(nv, nv, i, ic, (s), (a));				\
    } while (0);							\

int
oseen3_tri_stresslet(T *q, He *he, const real *x, const real *y, const real *z,
	real *oxx, real *oxy, real *oxz, real *oyy, real *oyz, real *ozz)
{
	real *nx, *ny, *nz, *area, A, s;
	int status, nv, nt, i;;

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
	nv = he_nv(he);
	nt = he_nt(he);
	matrix_zero(nv, nv, oxx);
	matrix_zero(nv, nv, oxy);
	matrix_zero(nv, nv, oxz);
	matrix_zero(nv, nv, oyy);
	matrix_zero(nv, nv, oyz);
	matrix_zero(nv, nv, ozz);
#pragma omp parallel for
	for (i = 0; i < nv; i++) {
	    int j, ia, ib, ic;
	    real a[3], b[3], c[3], point[3], center[3], normal[3];
	    real xx, xy, xz, yy, yz, zz;
	    i_vec_get(i, x, y, z, point);
	    for (j = 0; j < nt; j++) {
		he_tri_ijk(he, j, &ia, &ib, &ic);
		i_vec_get3(ia, ib, ic, x, y, z, a, b, c);
		tri_center(a, b, c, center);
		tri_normal(a, b, c, normal);
		A = tri_area(a, b, c)/(4*pi);
		stresslet(point, normal, center, &xx, &xy, &xz, &yy, &yz, &zz);
		TSET(i, A*xx, oxx);
		TSET(i, A*xy, oxy);
		TSET(i, A*xz, oxz);
		TSET(i, A*yy, oyy);
		TSET(i, A*yz, oyz);
		TSET(i, A*zz, ozz);
	    }
	}
	return CO_OK;
}
