#include <stdio.h>
#include <real.h>
#include <co/memory.h>
#include <co/err.h>

#include <alg/root.h>

#define FMT CO_REAL_OUT

#define T Arc
typedef struct T T;
int arc_velocity_ini(int, real , real, real (*)(real, void*), void*, T**);
int arc_points(T*, real**);
int arc_length(T*, real*);
int arc_fin(T*);
#undef T

#define T Arc
struct T
{
	real *points, length;
};

int
arc_velocity_ini(int n, real a, real b, real (*f)(real, void*), void *p, T **pq)
{
	T *q;
	real *points, length;

	MALLOC(1, &q);

	q->length = length;
	q->points = points;
	*pq = q;
	return CO_OK;
}

int
arc_fin(T *q)
{
	FREE(q->points);
	FREE(q);
	return CO_OK;
}

static real
f(real x, void *p)
{
	real alpha;
	alpha = *(real*)p;
	return (x - alpha)*(x + 2);
}

int
main(void)
{
	AlgRoot *q;
	real a, b, r, alpha;

	alg_root_ini(BRENT, &q);
	a = -1;
	b =  1;
	alpha = 0.01;
	alg_root_apply(q, a, b, f, &alpha, &r);
	printf(FMT "\n", r);
	fprintf(stderr, "%s %d\n", alg_root_name(q), alg_root_niter(q));

	alg_root_fin(q);
}

