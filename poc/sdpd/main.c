#include <stdio.h>
#include <stdlib.h>
#include <co/err.h>
#include <co/memory.h>

#pragma	src	"$HOME/co/lib"

#define T Ilist
typedef struct T T;

enum {
	ILIST_END = -1
};
int ilist_ini(T**);
int ilist_fin(T*);
int ilist_push(T*, int);
int ilist_len(T*);
int ilist_head(T*, int**);
int ilist_reset(T*);
int illist_fwrite(FILE*, T*);
struct T { /* size, head, tail */
	int s;
	int *h, *t;
};
enum {
	SIZE = 28, END = ILIST_END };
int ilist_ini(T **pq)
{
	T *q;

	MALLOC(1, &q);
	q->s = SIZE;
	MALLOC(q->s, &q->h);
	*(q->h) = END;
	q->t = q->h;
	*pq = q;
	return CO_OK;
}

int ilist_fin(T *q)
{
	FREE(q->h);
	FREE(q);
	return CO_OK;
}

int ilist_push(T *q, int x)
{
	int n;
	int s, *h, *t;

	s = q->s;
	h = q->h;
	t = q->t;
	n = t - h;
	if (n + 1 >= s) {
		s *= 2;
		REALLOC(s, &h);
		t = h + n;
	}
	*t = x;
	t++;
	*t = END;
	q->s = s;
	q->h = h;
	q->t = t;
	return CO_OK;
}

int ilist_len(T *q)
{
	int *t, *h, n;

	t = q->t;
	h = q->h;
	n = t - h;
	return n;
}

int ilist_head(T *q, int **ph)
{
	*ph = q->h;
	return CO_OK;
}

int ilist_reset(T *q)
{
	q->t = q->h;
	*(q->h) = END;
	return CO_OK;
}

int ilist_fwrite(FILE *f, T *q)
{
	int n, *a;

	ilist_head(q, &a);
	n = ilist_len(q);
	fprintf(f, "%d\n", n);
	while (*a != ILIST_END) {
		fprintf(f,  "%d\n", *(a++));
	}
	return CO_OK;
}

#undef T


#define T Alist
typedef struct T T;
enum {
	ALIST_END = ILIST_END
};
int alist_ini(int,  T**);
int alist_fin(T*);
int alist_reset(T*);
int alist_push(T*, int, int);
int alist_head(T*, int, int**);
int alist_len(T*, int);
int alist_n(T*);
int alist_fwrite(FILE*, T*);

struct T {
	int n;
	Ilist **a;
};

int alist_ini(int n, T **pq)
{
	T *q;
	int i;
	Ilist **a;
	if (n <= 0)
		ERR(CO_INDEX, "n=%d <= 0", n);
	MALLOC(1, &q);
	MALLOC(n,  &a);
	for (i = 0; i < n; i++)
		ilist_ini(&a[i]);
	q->n = n;
	q -> a = a;
	*pq = q;
	return CO_OK;
}

int alist_fin(T *q)
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

int alist_reset(T *q)
{
	int n, i;
	Ilist **a;
	n = q->n;
	a = q->a;
	for (i = 0; i < n; i++)
		ilist_reset(a[i]);
	return CO_OK;
}

int alist_push(T *q, int i, int j)
{
	int n;
	Ilist **a;
	a = q->a;
	n = q->n;

	if (i >= n)
		ERR(CO_INDEX, "i=%d >= n=%d", i, n);

	if (j >= n)
		ERR(CO_INDEX, "j=%d >= n=%d", j, n);

	if (i == j)
		ERR(CO_INDEX, "i == j=%d", i);

	ilist_push(a[i], j);
	ilist_push(a[j], i);

	return CO_OK;
}

int alist_head(T *q, int i, int **h)
{
	int n;
	Ilist **a;
	a = q->a;
	n = q->n;
	if (i >= n)
		ERR(CO_INDEX, "i=%d >= n=%d", i, n);

	return ilist_head(a[i], h);
}

int alist_len(T *q, int i)
{
	Ilist **a;
	a = q->a;
	return ilist_len(q->a[i]);
}

int alist_fwrite(FILE *f, T *q)
{
	int n, m, i, j;
	Ilist **a;
	int *b;

	n = q->n;
	a = q->a;

	fprintf(f, "%d\n", n);
	for (i = 0; i < n;  i++) {
		m = ilist_len(a[i]);
		ilist_head(a[i], &b);
		fprintf(f, "%d", m);
		for (j = 0; j < m; j++)
			fprintf(f, " %d", b[j]);
		fprintf(f, "\n");
	}
	return CO_OK;
}

int alist_n(T *q)
{
	return q->n;
}


#undef T

#define T Clist
typedef struct T T;
struct Clist {
	int n;
	Alist **a;
	Ilist **c; /* cells */
};

int clist_ini(int, T**);
int clist_alist(T*, Alist*);
int clist_fin(T*);
int clist_reset(T*);
int clist_push(T*, int cell, int part);
int clist_len(T*);
int clist_head(T*, int cell, int**);

int clist_ini(int n, T **pq)
{
	T *q;
	Ilist **c;
	int i;
	MALLOC(1, &q);
	MALLOC(n, &c);
	for (i = 0; i < n; i++)
		ilist_ini(&c[i]);

	q->n = n;
	q->c = c;
	*pq = q;
	return CO_OK;
}

int clist_free(T *q)
{
	int n, i;
	Ilist **c;
	n = q->n;
	c = q->c;
	for (i = 0; i < n; i++)
		ilist_fin(c[i]);
	FREE(c);
	FREE(q);
	return CO_OK;
}


/*
int main(void)
{
	int i;
	Ilist *a;
	ilist_ini(&a);

	for (i = 0; i < 20; i++)
		ilist_push(a, i);
	ilist_reset(a);
	ilist_push(a, 10);
	fprintf(stderr, "size: %d\n", a->s);
	ilist_fwrite(stdout, a);

	ilist_fin(a);
	return 0;
} */

int main(void)
{
	int n;
	Alist *alist;
	int *a;

	n = 4;
	alist_ini(n, &alist);
	alist_push(alist, 0, 1);
	alist_push(alist, 0, 2);
	alist_push(alist, 1, 2);

	alist_fwrite(stdout, alist);

	alist_fin(alist);

	return 0;
}


