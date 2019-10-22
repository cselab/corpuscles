#include <stdio.h>
#include <stdlib.h>
#include "co/err.h"
#include "co/memory.h"
#include "co/list/i.h"
#include "co/list/a.h"

#define T Alist

struct T {
	int n;
	Ilist **a;
};

int
alist_ini(int n, T ** pq)
{
	T *q;
	int i;
	Ilist **a;

	if (n <= 0)
		ERR(CO_INDEX, "n=%d <= 0", n);
	MALLOC(1, &q);
	MALLOC(n, &a);
	for (i = 0; i < n; i++)
		ilist_ini(&a[i]);
	q->n = n;
	q->a = a;
	*pq = q;
	return CO_OK;
}

int
alist_fin(T * q)
{
	int n, i;
	Ilist **a;

	n = q->n;
	a = q->a;
	for (i = 0; i < n; i++)
		ilist_fin(a[i]);
	FREE(a);
	FREE(q);
	return CO_OK;
}

int
alist_reset(T * q)
{
	int n, i;
	Ilist **a;

	n = q->n;
	a = q->a;
	for (i = 0; i < n; i++)
		ilist_reset(a[i]);
	return CO_OK;
}

int
alist_push(T * q, int i, int j)
{
	int n;
	Ilist **a;

	a = q->a;
	n = q->n;

	if (i >= n)
		ERR(CO_INDEX, "i=%d >= n=%d", i, n);
	if (i < 0)
		ERR(CO_INDEX, "i=%d < 0", i);

	if (j >= n)
		ERR(CO_INDEX, "j=%d >= n=%d", j, n);
	if (j < 0)
		ERR(CO_INDEX, "j=%d < 0", j);

	if (i == j)
		ERR(CO_INDEX, "i == j=%d", i);

	ilist_push(a[i], j);
	ilist_push(a[j], i);

	return CO_OK;
}

int
alist_push_uniq(T * q, int i, int j)
{
	int n;
	Ilist **a;

	a = q->a;
	n = q->n;

	if (i >= n)
		ERR(CO_INDEX, "i=%d >= n=%d", i, n);
	if (i < 0)
		ERR(CO_INDEX, "i=%d < 0", i);

	if (j >= n)
		ERR(CO_INDEX, "j=%d >= n=%d", j, n);
	if (j < 0)
		ERR(CO_INDEX, "j=%d < 0", j);

	if (i != j) {
		ilist_push_uniq(a[i], j);
		ilist_push_uniq(a[j], i);
	}
	return CO_OK;
}

int
alist_head(T * q, int i, int **h)
{
	int n;
	Ilist **a;

	a = q->a;
	n = q->n;
	if (i >= n)
		ERR(CO_INDEX, "i=%d >= n=%d", i, n);
	return ilist_head(a[i], h);
}

int
alist_len(T * q, int i)
{
	Ilist **a;

	a = q->a;
	return ilist_len(a[i]);
}

int
alist_fwrite(FILE * f, T * q)
{
	int n, m, i, j;
	Ilist **a;
	int *b;

	n = q->n;
	a = q->a;

	fprintf(f, "%d\n", n);
	for (i = 0; i < n; i++) {
		m = ilist_len(a[i]);
		ilist_head(a[i], &b);
		fprintf(f, "%d", m);
		for (j = 0; j < m; j++)
			fprintf(f, " %d", b[j]);
		fprintf(f, "\n");
	}
	return CO_OK;
}

int
alist_n(T * q)
{
	return q->n;
}
