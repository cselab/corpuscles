#include <stdio.h>
#include <stdlib.h>
#include <co/err.h>
#include <co/memory.h>

#include <co/list/i.h>
#include <co/list/a.h>

#define T Clist
typedef struct T T;
struct Clist {
	int n;
	Alist *alist;
	Ilist **c, **p; /* cells, particles */
	int own;
};

int clist_ini(int, T**);
int clist_alist(T*, Alist*);
int clist_fin(T*);
int clist_reset(T*);
int clist_push(T*, int cell, int part);
int clist_len(T*, int);
int clist_cells(T*, int cell, int**);
int clist_parts(T*, int cell, int**);

int clist_gen_n(int, T**);
int clist_gen_p(int, T**);
int clist_gen_nn(int, int, T**);
int clist_gen_np(int, int, T**);
int clist_gen_pn(int, int, T**);
int clist_gen_pp(int, int, T**);

int clist_ini(int n, T **pq)
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

int clist_fin(T *q)
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

int clist_reset(T *q)
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

int clist_alist(T *q, Alist *alist)
{
	int n, m;
	n = q->n;
	m = alist_n(alist);
	if (n != m)
		ERR(CO_INDEX, "n=%d != m=%d", n, m);
	q->alist = alist;
	return CO_OK;
}

int clist_alist_own(T *q, Alist *alist)
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

int clist_push(T *q, int cell, int part)
{
	int n, i;
	Ilist **c, **p;
	Alist *alist;
	int *a;

	n = q->n;
	c = q->c;
	p = q->p;
	alist = q->alist;

	if (alist == NULL) ERR(CO_INDEX, "alist == NULL");
	if (cell >= n) ERR(CO_INDEX, "cell=%d >=n=%d", cell, n);
	if (cell < 0) ERR(CO_INDEX, "cell=%d < 0", cell);

	ilist_push(c[cell], cell);
	ilist_push(p[cell], part);
	alist_head(alist, cell, &a);
	for (;;) {
		i = *(a++);
		if (i == ALIST_END)
			break;
		if (i >= n) ERR(CO_INDEX, "i=%d >= n=%d", i, n);
		if (i  < 0) ERR(CO_INDEX, "i=%d <0", i, n);
		ilist_push(c[i],  cell);
		ilist_push(p[i], part);
	}
	return CO_OK;
}

int clist_len(T *q, int cell)
{
	Ilist **p;
	p = q->p;
	return ilist_len(p[cell]);
}

int clist_cells(T *q, int cell, int **a)
{
	Ilist **c;
	c = q->c;
	return ilist_head(c[cell], a);
}

int clist_parts(T *q, int cell, int **a)
{
	Ilist **p;
	p = q->p;
	return ilist_head(p[cell], a);
}

int clist_fwrite(FILE *f, T *q)
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

int clist_gen_n(int k, T **pq)
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

int clist_gen_p(int k, T **pq)
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
	{-1, -1},
	{-1, 0},
	{-1, 1},
	{0, -1},
	{0, 1},
	{1, -1},
	{1, 0},
	{1, 1},
};
static int bc_nn(int K, int M, int *i, int *j)
{
	return 0 <= *i && *i < K && 0 <= *j && *j < M;
}
static int bc_pn(int K, int M, int *i, int *j)
{
	if (*i < 0) *i += K;
	if (*i >= K) *i -= K;
	return  0 <= *j && *j < M;
}
static int bc_np(int K, int M, int *i, int *j)
{
	if (*j < 0) *j += M;
	if (*j >= M) *j -= M;
	return  0 <= *i && *i < K;
}
static int bc_pp(int K, int M, int *i, int *j)
{
	if (*i < 0) *i += K;
	if (*i >= K) *i -= K;
	if (*j < 0) *j += M;
	if (*j >= M) *j -= M;
	return 1;
}
static int gen2(int K,  int M,  int (*bc)(int, int, int*, int*), T **pq)
{
	int nd = sizeof(dd)/sizeof(dd[0]);
	int k, m, l, *d, i, j;
	T *q;
	Alist *a;
	alist_ini(K*M, &a);
	for (k = 0; k < K; k++)
		for (m = 0; m < M; m++)
			for (l = 0; l < nd; l++) {
				d = dd[l];
				i = k + d[X];
				j = m + d[Y];
				if (bc(K, M, &i, &j))
					alist_push_uniq(a, i*M + j,  k*M + m);
			}
	MALLOC(1, &q);
	clist_ini(K*M, &q);
	clist_alist_own(q, a);
	*pq = q;
	return CO_OK;
}

int clist_gen_nn(int K, int M, T **pq)
{
	return gen2(K, M, bc_nn, pq);
}

int clist_gen_pn(int K, int M, T **pq)
{
	return gen2(K, M, bc_pn, pq);
}

int clist_gen_np(int K, int M, T **pq)
{
	return gen2(K, M, bc_np, pq);
}

int clist_gen_pp(int K, int M, T **pq)
{
	return gen2(K, M, bc_pp, pq);
}

int main(void)
{
	int n;
	Alist *alist;
	Clist *clist;

	n = 5;
	alist_ini(n, &alist);
	alist_push(alist, 0, 1);
	alist_push(alist, 0, 2);
	alist_push(alist, 1, 2);
	alist_push(alist, 3, 4);

	clist_ini(n, &clist);
	clist_alist(clist, alist);

	clist_push(clist, 0, 100);
	clist_push(clist, 1, 101);
	clist_push(clist, 3, 103);
	clist_fwrite(stdout, clist);

	clist_fin(clist);
	alist_fin(alist);

	return 0;
}

