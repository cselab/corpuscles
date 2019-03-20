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
		alist_push(a, i, i + 1);
	MALLOC(1, &q);
	clist_ini(k, &q);
	clist_alist_own(q, a);
	*pq = q;
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
}

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


*/

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
	/* clist_reset(clist); */
	clist_fwrite(stdout, clist);

	clist_fin(clist);
	alist_fin(alist);

	return 0;
}
