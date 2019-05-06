#include <stdio.h>

#include "real.h"
#include "co/argv.h"
#include "co/err.h"
#include "co/f2/glen.h"
#include "co/memory.h"
#include "co/skel.h"
#include "co/vec2.h"
#include "co/edg2.h"
#include "co/sum.h"

#define T F2Glen

struct T
{
	int n;
	real *len;
	real l, k, L;
};

int
f2_glen_ini(real l, real k, Skel *skel, T **pq)
{
	T *q;
	real *len;
	int n;

	if (l <= 0)
		ERR(CO_NUM, "l <= 0");
	MALLOC(1, &q);
	n = skel_ne(skel);
	MALLOC(n, &len);
	q->n = n;
	q->l = l;
	q->k = k;
	*pq = q;
	return CO_OK;
}

int
f2_glen_argv(char ***pv, Skel *skel, T **pq)
{
	return CO_OK;
}

int f2_glen_fin(T *q)
{
	FREE(q->len);
	FREE(q);
	return CO_OK;
}

real
f2_glen_energy(T *q, Skel *skel, const real *x, const real *y)
{
	int e, i, j, n;
	real L, k, l;
	real a[2], b[2];
	HeSum *sum;

	n = skel_ne(skel);
	l = q->l;
	k = q->k;
	he_sum_ini(&sum);
	for (e = 0; e < n; i++) {
		skel_edg_ij(skel, e, &i, &j);
		vec2_get(i, x, y, a);
		vec2_get(j, x, y, b);
		he_sum_add(sum, edg2_abs(a, b));
	}
	L = he_sum_get(sum);
	he_sum_fin(sum);
	q->L = L;
	return k*(L - l)*(L - l)/l;
}

int
f2_glen_force(T *q, Skel *skel, const real *x, const real *y, real *u, real *v)
{
	return CO_OK;
}

real
f2_glen_len(T *q)
{
	return q->L;
}
