#include <stdio.h>

#include "real.h"
#include "co/argv.h"
#include "co/err.h"
#include "co/f2/len.h"
#include "co/macro.h"
#include "co/memory.h"
#include "co/skel.h"
#include "co/vec2.h"
#include "co/edg2.h"
#include "co/dedg2.h"
#include "co/sum.h"

#define T F2Len

struct T
{
	int n;
	real Ka, a3, a4;
};

int
f2_len_ini(FILE *f, real Ka, real a3, real a4, T **pq)
{
	/*T *q;
	int n;

	if (l <= 0)
		ERR(CO_NUM, "l <= 0");
	MALLOC(1, &q);
	n = skel_ne(skel);
	q->n = n;
	q->l = l;
	q->k = k;
	*pq = q; */
	return CO_OK;
}

int
f2_len_argv(char ***p, __UNUSED Skel *skel, T **pq)
{
	int status;
	FILE *f;
	real Ka, a3, a4;
	if ((status = argv_real(p, &Ka)) != CO_OK)
		return status;
	if ((status = argv_real(p, &a3)) != CO_OK)
		return status;
	if ((status = argv_real(p, &a4)) != CO_OK)
		return status;
	return f2_len_ini(f, Ka, a3, a4, pq);
}

int f2_len_fin(T *q)
{
	FREE(q);
	return CO_OK;
}

real
compute_len(Skel *skel, const real *x, const real *y)
{
	int e, i, j, n;
	HeSum *sum;
	real a[2], b[2], L;

	n = skel_ne(skel);
	he_sum_ini(&sum);
	for (e = 0; e < n; e++) {
		skel_edg_ij(skel, e, &i, &j);
		vec2_get(i, x, y, a);
		vec2_get(j, x, y, b);
		he_sum_add(sum, edg2_abs(a, b));
	}
	L = he_sum_get(sum);
	he_sum_fin(sum);
	return L;
}

real
f2_len_energy(T *q, Skel *skel, const real *x, const real *y)
{
	real L, k, l;

	/* l = q->l;
	k = q->k;
	L = compute_len(skel, x, y);
	q->L = L;
	return k*(L - l)*(L - l)/l; */
	return 0;
}

int
f2_len_force(T *q, Skel *skel, const real *x, const real *y, real *u, real *v)
{
	int e, i, j, n;
	real L, k, l, coeff;
	real a[2], b[2], da[2], db[2];
	/*
	n = skel_ne(skel);
	l = q->l;
	k = q->k;
	L = compute_len(skel, x, y);
	coeff = 2*k*(L - l)/l;
	for (e = 0; e < n; e++) {
		skel_edg_ij(skel, e, &i, &j);
		vec2_get(i, x, y, a);
		vec2_get(j, x, y, b);
		dedg2_abs(a, b, da, db);
		vec2_scalar_append(da, coeff, i, u, v);
		vec2_scalar_append(db, coeff, j, u, v);
	} */
	return CO_OK;
}

