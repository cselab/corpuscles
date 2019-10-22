#include <stdio.h>

#include "real.h"
#include "co/argv.h"
#include "co/memory.h"
#include "co/err.h"
#include "co/he.h"
#include "co/vec.h"
#include "co/tri.h"
#include "co/dtri.h"

#include "co/f/area_sq.h"

#define T HeFAreaSq

struct T {
	int n;
	real *area;
	real K;
};

int
he_f_area_sq_ini(real K, He * he, T ** pq)
{
	T *q;
	int n;

	MALLOC(1, &q);
	n = he_nt(he);

	MALLOC(n, &q->area);

	q->n = n;
	q->K = K;

	*pq = q;
	return CO_OK;
}

int
he_f_area_sq_argv(char ***p, He * he, T ** pq)
{
	int status;
	real x;

	if ((status = argv_real(p, &x)) != CO_OK)
		return status;
	return he_f_area_sq_ini(x, he, pq);
}

int
he_f_area_sq_fin(T * q)
{
	FREE(q->area);
	FREE(q);
	return CO_OK;
}

int
he_f_area_sq_a(T * q, /**/ real ** pa)
{
	*pa = q->area;
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
compute_force(real K, real * darea, He * he, const real * x,
			  const real * y, const real * z, /**/ real * fx, real * fy,
			  real * fz)
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
		coeff = 2 * K * darea[t];
		vec_scalar_append(da, coeff, i, /**/ fx, fy, fz);
		vec_scalar_append(db, coeff, j, /**/ fx, fy, fz);
		vec_scalar_append(dc, coeff, k, /**/ fx, fy, fz);
	}
}

int
he_f_area_sq_force(T * q, He * he,
				   const real * x, const real * y, const real * z, /**/
				   real * fx, real * fy, real * fz)
{
	int n;
	real *area, K;

	n = q->n;
	area = q->area;
	K = q->K;

	if (he_nt(he) != n)
		ERR(CO_INDEX, "he_nt(he)=%d != n = %d", he_nt(he), n);

	compute_area(he, x, y, z, /**/ area);
	compute_force(K, area, he, x, y, z, /**/ fx, fy, fz);

	return CO_OK;
}

static real
compute_energy(real K, real * darea, int n)
{
	int t;
	real da, e;

	e = 0;
	for (t = 0; t < n; t++) {
		da = darea[t];
		e += da * da;
	}
	return K * e;
}

real
he_f_area_sq_energy(T * q, He * he,
					const real * x, const real * y, const real * z)
{
	int n;
	real *area, K;

	n = q->n;
	area = q->area;
	K = q->K;

	if (he_nt(he) != n)
		ERR(CO_INDEX, "he_nt(he)=%d != n = %d", he_nt(he), n);

	compute_area(he, x, y, z, /**/ area);
	return compute_energy(K, area, n);
}
