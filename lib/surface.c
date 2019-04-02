#include <stdio.h>
#include "real.h"

#include "co/bbox.h"
#include "co/err.h"
#include "co/he.h"
#include "co/memory.h"
#include "co/predicate.h"
#include "co/vec.h"

#include "co/surface.h"

enum {
	X, Y, Z
};
#define T Surface
struct T {
	Bbox *bbox;
	He *he;
	const real *x, *y, *z;
};

int
surface_ini(T **pq)
{
	T *q;
	MALLOC(1, &q);
	bbox_ini(&q->bbox);
	predicate_ini();
	*pq = q;
	return CO_OK;
}

int
surface_fin(T *q)
{
	bbox_fin(q->bbox);
	FREE(q);
	return CO_OK;
}

int
surface_update(T *q, He *he, const real *x, const real *y, const real *z)
{	
	int n;

	q->he = he;
	q->x = x;
	q->y = y;
	q->z = z;
	n = he_nv(he);
	bbox_update(q->bbox, n, x, y, z);
	return CO_OK;
}

int
surface_inside(T *q, real u, real v, real w)
{
	int n, t, i, j, k, m;
	He *he;
	Bbox *bbox;
	const real *x, *y, *z;
	real a[3], b[3], c[3], d[3], e[3];

	he = q->he;
	x = q->x; y = q->y; z = q->z;
	bbox = q->bbox;
	vec_ini(u, v, w, /**/ d);
	vec_ini(bbox_xhi(bbox), v, w, /**/ e);
	
	n = he_nt(he);
	for (t = m = 0; t < n; t++) {
		he_tri_ijk(he, t, &i, &j, &k);
		vec_get(i, x, y, z, a);
		vec_get(j, x, y, z, b);
		vec_get(k, x, y, z, c);
		m += predicate_ray(d, e,   a, b, c);
	}
	
	return m % 2;
}