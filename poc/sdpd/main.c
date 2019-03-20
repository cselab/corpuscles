#include <stdio.h>
#include <co/err.h>
#include <co/memory.h>

#pragma	src	"$HOME/co/lib"

#define T Ilist

typedef struct T T;
struct T { /* size, head, tail */
	int s;
	int *h, *t;
};

int ilist_ini(T**);
int ilist_fin(T*);
int ilist_push(T*, int);
int ilist_size(T*);
int ilist_head(T*, int**);
int ilist_reset(T*);

enum {
	SIZE = 16
};

int ilist_ini(T **pq)
{
	T *q;

	MALLOC(1, &q);
	q->s = SIZE;
	MALLOC(q->s, &q->h);
	*(q->h) = -1;
	q->t = q->h;
	*pq = q;
	return CO_OK;
}

int ilist_fin(T *q) {
	FREE(q->h);
	FREE(q);
	return CO_OK;
}

int main(void)
{
	printf("hello a_list\n");
}



