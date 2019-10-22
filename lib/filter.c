#include <stdio.h>

#include "real.h"
#include "co/memory.h"
#include "co/err.h"
#include "co/he.h"
#include "co/vec.h"
#include "co/tri.h"
#include "co/filter.h"

#define T Filter

struct T {
	real *B, *sum;
};

int
filter_ini(He * he, T ** pq)
{
	T *q;
	int n;

	MALLOC(1, &q);

	n = he_nv(he);
	MALLOC(n, &q->B);
	MALLOC(n, &q->sum);

	*pq = q;
	return CO_OK;
}

int
filter_fin(T * q)
{
	FREE(q->B);
	FREE(q->sum);
	FREE(q);
	return CO_OK;
}

static int
zero(int n, real * a)
{
	int i;

	for (i = 0; i < n; i++)
		a[i] = 0;
	return CO_OK;
}

static int
div(int n, const real * a, const real * b, /*io */ real * c)
{
	int i;

	for (i = 0; i < n; i++) {
		if (b[i] == 0)
			ERR(CO_NUM, "b[%d] == 0", i);
		c[i] = a[i] / b[i];
	}
	return CO_OK;
}

static int
get3(const real * x, const real * y, const real * z,
	 int i, int j, int k, /**/ real a[3], real b[3], real c[3])
{
	vec_get(i, x, y, z, /**/ a);
	vec_get(j, x, y, z, /**/ b);
	vec_get(k, x, y, z, /**/ c);
	return CO_OK;
}

int
filter_apply(T * q, He * he,
			 const real * x, const real * y, const real * z,
			 /*io */ real * A)
{
	int nv, nh, h, i, j, k;
	real u;
	real *B, *sum;
	real a[3], b[3], c[3];

	nv = he_nv(he);
	nh = he_nh(he);
	B = q->B;
	sum = q->sum;

	zero(nv, B);
	zero(nv, sum);

	for (h = 0; h < nh; h++) {
		he_ijk(he, h, &i, &j, &k);
		get3(x, y, z, i, j, k, a, b, c);
		u = tri_angle(c, a, b);
		B[i] += (A[k] + A[i] + A[j]) * u;
		sum[i] += 3 * u;
	}

	div(nv, B, sum, A);
	return CO_OK;
}
