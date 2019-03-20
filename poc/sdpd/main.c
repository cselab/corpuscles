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


struct T { /* size, head, tail */
	int s;
	int *h, *t;
};

enum {
	SIZE = 16, END = ILIST_END
};

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
	if (n >= s) {
		s *= 2;
		h = realloc(&h, s);
		t = h + n;
	}
	*(t++) = x;
	*(t++) = END;
	q->s = s;
	q->h = h;
	q->t = t;
	return CO_OK;
}

int ilist_len(T *q)
{
	return q->t - q->h;
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

int illist_fwrite(FILE *f, T *q) {
	int n, *a;

	ilist_head(q, &a);
	n = ilist_len(q);
	fprintf(f, "%d\n", n);
	while (*a != ILIST_END) {
		fprintf(f,  "%d\n", *a);
	}
}

int main(void)
{
	printf("hello a_list\n");
}

