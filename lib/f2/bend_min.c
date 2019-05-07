#include <stdio.h>
#include <tgmath.h>

#include "real.h"
#include "co/argv.h"
#include "co/dvec2.h"
#include "co/edg2.h"
#include "co/err.h"
#include "co/f2/bend_min.h"
#include "co/memory.h"
#include "co/skel.h"
#include "co/sum.h"
#include "co/tri2.h"
#include "co/vec2.h"

#define T F2BendMin

struct T
{
	int n;
	real k;
};

int
f2_bend_min_ini(real k, Skel *skel, T **pq)
{
	T *q;
	int n;
	MALLOC(1, &q);
	n = skel_ne(skel);
	q->n = n;
	q->k = k;
	*pq = q;
	return CO_OK;
}

int
f2_bend_min_argv(char ***p, Skel *skel, T **pq)
{
	int status;
	real x;
	if ((status = argv_real(p, &x)) != CO_OK)
		return status;
	return f2_bend_min_ini(x, skel, pq);
}

int f2_bend_min_fin(T *q)
{
	FREE(q);
	return CO_OK;
}

real
compute_energy(Skel *skel, const real *x, const real *y)
{
	int v, i, j, k, n;
	HeSum *sum;
	real a[2], b[2], c[2], u, w, h, E;

	n = skel_nv(skel);
	he_sum_ini(&sum);
	for (v = 0; v < n; v++) {
		if (skel_bnd(skel, v)) continue;
		skel_ver_ijk(skel, v, &i, &j, &k);
		vec2_get(i, x, y, a);
		vec2_get(j, x, y, b);
		vec2_get(j, x, y, c);
		u = edg2_abs(a, b);
		w = edg2_abs(b, c);
		h = tri2_angle(a, b, c)/(u + w)/4;
		he_sum_add(sum, h*h);
	}
	E = he_sum_get(sum);
	he_sum_fin(sum);
	return E;
}

real
f2_bend_min_energy(T *q, Skel *skel, const real *x, const real *y)
{
	real E, k;

	k = q->k;
	E = compute_energy(skel, x, y);
	return k*E;
}

int
f2_bend_min_force(T *q, Skel *skel, const real *x, const real *y, real *u, real *v)
{
	int e, i, j, n;
	real E, k, coeff;
	real a[2], b[2], da[2], db[2];

	n = skel_nv(skel);
	k = q->k;
	E = compute_energy(skel, x, y);
	coeff = k;
	for (e = 0; e < n; e++) {
		skel_edg_ij(skel, e, &i, &j);
		vec2_get(i, x, y, a);
		vec2_get(j, x, y, b);
		dvec2_cross(b, a, db, da);
		vec2_scalar_append(da, coeff, i, u, v);
		vec2_scalar_append(db, coeff, j, u, v);
	}
	return CO_OK;
}

