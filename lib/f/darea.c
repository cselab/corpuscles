#include <stdio.h>

#include "real.h"

#include "co/argv.h"
#include "co/dtri.h"
#include "co/err.h"
#include "co/f/darea.h"
#include "co/he.h"
#include "co/macro.h"
#include "co/memory.h"
#include "co/tri.h"
#include "co/vec.h"

#define T HeFDarea

struct T {
	int n;
	real *area;
};

int
he_f_darea_ini(He * he, T ** pq)
{
	T *q;
	int n;

	MALLOC(1, &q);
	n = he_nt(he);

	MALLOC(n, &q->area);

	q->n = n;

	*pq = q;
	return CO_OK;
}

int
he_f_darea_argv(__UNUSED char ***p, He * he, T ** pq)
{
	return he_f_darea_ini(he, pq);
}

int
he_f_darea_fin(T * q)
{
	FREE(q->area);
	FREE(q);
	return CO_OK;
}


static void
get(int t, He * he,
	const real * x, const real * y, const real * z, /**/
	real a[3], real b[3], real c[3])
{
	int i, j, k;

	he_tri_ijk(he, t, &i, &j, &k);
	vec_get(i, x, y, z, /**/ a);
	vec_get(j, x, y, z, /**/ b);
	vec_get(k, x, y, z, /**/ c);
}

static void
compute_area(He * he, const real * x, const real * y, const real * z,
			 /**/ real * area)
{
	real a[3], b[3], c[3];
	int n, t;

	n = he_nt(he);
	for (t = 0; t < n; t++) {
		get(t, he, x, y, z, /**/ a, b, c);
		area[t] = tri_area(a, b, c);
	}
}

static void
compute_force(He * he, const real * x, const real * y, const real * z,
			  /**/ real * fx, real * fy, real * fz)
{
	int n, t, i, j, k;
	real a[3], b[3], c[3], da[3], db[3], dc[3], coeff;

	n = he_nt(he);
	for (t = 0; t < n; t++) {
		he_tri_ijk(he, t, /**/ &i, &j, &k);
		vec_get(i, x, y, z, /**/ a);
		vec_get(j, x, y, z, /**/ b);
		vec_get(k, x, y, z, /**/ c);
		dtri_area(a, b, c, /**/ da, db, dc);
		coeff = 1.0;
		vec_scalar_append(da, coeff, i, /**/ fx, fy, fz);
		vec_scalar_append(db, coeff, j, /**/ fx, fy, fz);
		vec_scalar_append(dc, coeff, k, /**/ fx, fy, fz);
	}
}

int
he_f_darea_force(T * q, He * he,
				 const real * x, const real * y, const real * z, /**/
				 real * fx, real * fy, real * fz)
{
	int n;

	n = q->n;

	if (he_nt(he) != n)
		ERR(CO_INDEX, "he_nt(he)=%d != n = %d", he_nt(he), n);

	compute_force(he, x, y, z, /**/ fx, fy, fz);

	return CO_OK;
}

static real
compute_energy(real * area, int n)
{
	int t;
	real e;

	e = 0;
	for (t = 0; t < n; t++) {
		e += area[t];
	}
	return e;
}

real
he_f_darea_energy(T * q, He * he,
				  const real * x, const real * y, const real * z)
{
	int n;
	real *area;

	n = q->n;
	area = q->area;

	if (he_nt(he) != n)
		ERR(CO_INDEX, "he_nt(he)=%d != n = %d", he_nt(he), n);

	compute_area(he, x, y, z, /**/ area);
	return compute_energy(area, n);
}
