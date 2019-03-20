#include <stdio.h>
#include <stdlib.h>
#include <co/err.h>
#include <co/memory.h>

#pragma	src	"$HOME/co/lib"

#define T Ilist
typedef struct T T;

enum { ILIST_END = -1 };
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
enum { SIZE = 28, END = ILIST_END };
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
	int s, *h, *t, *cp;

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

int ilist_fwrite(FILE *f, T *q) {
	int n, *a;

	ilist_head(q, &a);
	n = ilist_len(q);
	fprintf(f, "%d\n", n);
	while (*a != ILIST_END) {
		fprintf(f,  "%d\n", *(a++));
	}
}



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


