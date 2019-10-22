#include <stdio.h>
#include <stdlib.h>

#include <co/err.h>
#include <co/memory.h>

#include "co/list/i.h"
#include "co/list/a.h"
#include "co/list/c.h"

#define T Clist
typedef struct T T;
struct Clist {
	int n;
	Alist *alist;
	Ilist **c, **p;				/* cells, particles */
	int own;
};

int
clist_ini(int n, T ** pq)
{
	T *q;
	Ilist **c, **p;
	int i;

	MALLOC(1, &q);
	MALLOC(n, &c);
	MALLOC(n, &p);
	for (i = 0; i < n; i++) {
		ilist_ini(&c[i]);
		ilist_ini(&p[i]);
	}
	q->n = n;
	q->c = c;
	q->p = p;
	q->alist = NULL;
	q->own = 0;
	*pq = q;
	return CO_OK;
}

int
clist_fin(T * q)
{
	int n, i;
	Ilist **c, **p;

	n = q->n;
	c = q->c;
	p = q->p;
	for (i = 0; i < n; i++) {
		ilist_fin(c[i]);
		ilist_fin(p[i]);
	}
	if (q->own)
		alist_fin(q->alist);
	FREE(c);
	FREE(p);
	FREE(q);
	return CO_OK;
}

int
clist_reset(T * q)
{
	int n, i;
	Ilist **c, **p;

	n = q->n;
	c = q->c;
	p = q->p;
	for (i = 0; i < n; i++) {
		ilist_reset(c[i]);
		ilist_reset(p[i]);
	}
	return CO_OK;
}

int
clist_alist(T * q, Alist * alist)
{
	int n, m;

	n = q->n;
	m = alist_n(alist);
	if (n != m)
		ERR(CO_INDEX, "n=%d != m=%d", n, m);
	q->alist = alist;
	return CO_OK;
}

int
clist_alist_own(T * q, Alist * alist)
{
	int n, m;

	n = q->n;
	m = alist_n(alist);
	if (n != m)
		ERR(CO_INDEX, "n=%d != m=%d", n, m);
	q->alist = alist;
	q->own = 1;
	return CO_OK;
}

int
clist_push(T * q, int cell, int part)
{
	int n, i;
	Ilist **c, **p;
	Alist *alist;
	int *a;

	n = q->n;
	c = q->c;
	p = q->p;
	alist = q->alist;

	if (alist == NULL)
		ERR(CO_INDEX, "alist == NULL");
	if (cell >= n)
		ERR(CO_INDEX, "cell=%d >=n=%d", cell, n);
	if (cell < 0)
		ERR(CO_INDEX, "cell=%d < 0", cell);

	ilist_push(c[cell], cell);
	ilist_push(p[cell], part);
	alist_head(alist, cell, &a);
	for (;;) {
		i = *(a++);
		if (i == ALIST_END)
			break;
		if (i >= n)
			ERR(CO_INDEX, "i=%d >= n=%d", i, n);
		if (i < 0)
			ERR(CO_INDEX, "i=%d <0", i, n);
		ilist_push(c[i], cell);
		ilist_push(p[i], part);
	}
	return CO_OK;
}

int
clist_len(T * q, int cell)
{
	Ilist **p;
	int n;

	p = q->p;
	n = q->n;
	if (cell >= n)
		ERR(CO_INDEX, "cell=%d >= n=%d", cell, n);
	if (cell < 0)
		ERR(CO_INDEX, "cell=%d < 0", cell);
	return ilist_len(p[cell]);
}

int
clist_cells(T * q, int cell, int **a)
{
	Ilist **c;
	int n;

	c = q->c;
	n = q->n;
	if (cell >= n)
		ERR(CO_INDEX, "cell=%d >= n=%d", cell, n);
	if (cell < 0)
		ERR(CO_INDEX, "cell=%d < 0", cell);
	return ilist_head(c[cell], a);
}

int
clist_parts(T * q, int cell, int **a)
{
	Ilist **p;
	int n;

	p = q->p;
	n = q->n;
	if (cell >= n)
		ERR(CO_INDEX, "cell=%d >= n=%d", cell, n);
	if (cell < 0)
		ERR(CO_INDEX, "cell=%d < 0", cell);
	return ilist_head(p[cell], a);
}

int
clist_fwrite(FILE * f, T * q)
{
	int n, m, i, j;
	int *cells, *parts;

	n = q->n;
	fprintf(f, "%d\n", n);
	for (i = 0; i < n; i++) {
		m = clist_len(q, i);
		fprintf(f, "%d", m);
		clist_cells(q, i, &cells);
		clist_parts(q, i, &parts);
		for (j = 0; j < m; j++)
			fprintf(f, " %d:%d", cells[j], parts[j]);
		fprintf(f, "\n");
	}
	return CO_OK;
}

int
clist_gen_n(int k, T ** pq)
{
	int i;
	T *q;
	Alist *a;

	alist_ini(k, &a);
	for (i = 0; i < k - 1; i++)
		alist_push_uniq(a, i, i + 1);
	MALLOC(1, &q);
	clist_ini(k, &q);
	clist_alist_own(q, a);
	*pq = q;
	return CO_OK;
}

int
clist_gen_p(int k, T ** pq)
{
	int i;
	T *q;
	Alist *a;

	alist_ini(k, &a);
	for (i = 0; i < k - 1; i++)
		alist_push_uniq(a, i, i + 1);
	alist_push(a, 0, k - 1);
	MALLOC(1, &q);
	clist_ini(k, &q);
	clist_alist_own(q, a);
	*pq = q;
	return CO_OK;
}

enum {
	X, Y, Z
};

static int dd[][2] = {
	{ -1, -1 },
	{ -1, 0 },
	{ -1, 1 },
	{ 0, -1 },
	{ 0, 1 },
	{ 1, -1 },
	{ 1, 0 },
	{ 1, 1 },
};

static int
bc_nn(int K, int M, int *i, int *j)
{
	return 0 <= *i && *i < K && 0 <= *j && *j < M;
}

static int
bc_pn(int K, int M, int *i, int *j)
{
	if (*i < 0)
		*i += K;
	if (*i >= K)
		*i -= K;
	return 0 <= *j && *j < M;
}

static int
bc_np(int K, int M, int *i, int *j)
{
	if (*j < 0)
		*j += M;
	if (*j >= M)
		*j -= M;
	return 0 <= *i && *i < K;
}

static int
bc_pp(int K, int M, int *i, int *j)
{
	if (*i < 0)
		*i += K;
	if (*i >= K)
		*i -= K;
	if (*j < 0)
		*j += M;
	if (*j >= M)
		*j -= M;
	return 1;
}

static int
gen2(int K, int M, int (*bc)(int, int, int *, int *), T ** pq)
{
	int nd = sizeof(dd) / sizeof(dd[0]);
	int k, m, l, *d, i, j;
	T *q;
	Alist *a;

	alist_ini(K * M, &a);
	for (k = 0; k < K; k++)
		for (m = 0; m < M; m++)
			for (l = 0; l < nd; l++) {
				d = dd[l];
				i = k + d[X];
				j = m + d[Y];
				if (bc(K, M, &i, &j))
					alist_push_uniq(a, i * M + j, k * M + m);
			}
	MALLOC(1, &q);
	clist_ini(K * M, &q);
	clist_alist_own(q, a);
	*pq = q;
	return CO_OK;
}

int
clist_gen_nn(int K, int M, T ** pq)
{
	return gen2(K, M, bc_nn, pq);
}

int
clist_gen_pn(int K, int M, T ** pq)
{
	return gen2(K, M, bc_pn, pq);
}

int
clist_gen_np(int K, int M, T ** pq)
{
	return gen2(K, M, bc_np, pq);
}

int
clist_gen_pp(int K, int M, T ** pq)
{
	return gen2(K, M, bc_pp, pq);
}

static int ee[][3] = {
	{ -1, -1, -1 },
	{ -1, -1, 0 },
	{ -1, -1, 1 },
	{ -1, 0, -1 },
	{ -1, 0, 0 },
	{ -1, 0, 1 },
	{ -1, 1, -1 },
	{ -1, 1, 0 },
	{ -1, 1, 1 },
	{ 0, -1, -1 },
	{ 0, -1, 0 },
	{ 0, -1, 1 },
	{ 0, 0, -1 },
	{ 0, 0, 1 },
	{ 0, 1, -1 },
	{ 0, 1, 0 },
	{ 0, 1, 1 },
	{ 1, -1, -1 },
	{ 1, -1, 0 },
	{ 1, -1, 1 },
	{ 1, 0, -1 },
	{ 1, 0, 0 },
	{ 1, 0, 1 },
	{ 1, 1, -1 },
	{ 1, 1, 0 },
	{ 1, 1, 1 },
};

static int
bc_nnn(int K, int M, int N, int *k, int *m, int *n)
{
	return 0 <= *k && *k < K && 0 <= *m && *m < M && 0 <= *n && *n < N;
}

static int
bc_pnn(int K, int M, int N, int *k, int *m, int *n)
{
	if (*k < 0)
		*k += K;
	if (*k >= K)
		*k -= K;
	return 0 <= *m && *m < M && 0 <= *n && *n < N;
}

static int
bc_nnp(int K, int M, int N, int *k, int *m, int *n)
{
	if (*n < 0)
		*n += N;
	if (*n >= N)
		*n -= N;
	return 0 <= *k && *k < K && 0 <= *m && *m < M;
}

static int
bc_npn(int K, int M, int N, int *k, int *m, int *n)
{
	if (*m < 0)
		*m += M;
	if (*m >= M)
		*m -= M;
	return 0 <= *k && *k < K && 0 <= *n && *n < N;
}

static int
bc_ppn(int K, int M, int N, int *k, int *m, int *n)
{
	if (*k < 0)
		*k += K;
	if (*k >= K)
		*k -= K;
	if (*m < 0)
		*m += M;
	if (*m >= M)
		*m -= M;
	return 0 <= *n && *n < N;
}

static int
bc_npp(int K, int M, int N, int *k, int *m, int *n)
{
	if (*m < 0)
		*m += M;
	if (*m >= M)
		*m -= M;
	if (*n < 0)
		*n += N;
	if (*n >= N)
		*n -= N;
	return 0 <= *k && *k < K;
}

static int
bc_pnp(int K, int M, int N, int *k, int *m, int *n)
{
	if (*k < 0)
		*k += K;
	if (*k >= K)
		*k -= K;
	if (*n < 0)
		*n += N;
	if (*n >= N)
		*n -= N;
	return 0 <= *m && *m < M;
}

static int
bc_ppp(int K, int M, int N, int *k, int *m, int *n)
{
	if (*k < 0)
		*k += K;
	if (*k >= K)
		*k -= K;
	if (*m < 0)
		*m += M;
	if (*m >= M)
		*m -= M;
	if (*n < 0)
		*n += N;
	if (*n >= N)
		*n -= N;
	return 1;
}

static int
gen3(int K, int M, int N, int (*bc)(int, int, int, int *, int *, int *),
	 T ** pq)
{
	int nd = sizeof(ee) / sizeof(ee[0]);
	int l, *d, x, y, z, i, j, k;
	T *q;
	Alist *a;

	alist_ini(K * M * N, &a);
	for (x = 0; x < K; x++)
		for (y = 0; y < M; y++)
			for (z = 0; z < N; z++)
				for (l = 0; l < nd; l++) {
					d = ee[l];
					i = x + d[X];
					j = y + d[Y];
					k = z + d[Z];
					if (bc(K, M, N, &i, &j, &k))
						alist_push_uniq(a, i * M * N + j * N + k,
										x * M * N + y * N + z);
				}
	clist_ini(K * M * N, &q);
	clist_alist_own(q, a);
	*pq = q;
	return CO_OK;
}

int
clist_gen_nnn(int K, int M, int N, T ** pq)
{
	return gen3(K, M, N, bc_nnn, pq);
}

int
clist_gen_pnn(int K, int M, int N, T ** pq)
{
	return gen3(K, M, N, bc_pnn, pq);
}

int
clist_gen_npn(int K, int M, int N, T ** pq)
{
	return gen3(K, M, N, bc_npn, pq);
}

int
clist_gen_nnp(int K, int M, int N, T ** pq)
{
	return gen3(K, M, N, bc_nnp, pq);
}

int
clist_gen_ppn(int K, int M, int N, T ** pq)
{
	return gen3(K, M, N, bc_ppn, pq);
}

int
clist_gen_pnp(int K, int M, int N, T ** pq)
{
	return gen3(K, M, N, bc_pnp, pq);
}

int
clist_gen_npp(int K, int M, int N, T ** pq)
{
	return gen3(K, M, N, bc_npp, pq);
}

int
clist_gen_ppp(int K, int M, int N, T ** pq)
{
	return gen3(K, M, N, bc_ppp, pq);
}
