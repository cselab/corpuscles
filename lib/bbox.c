#include <stdio.h>
#include "real.h"
#include "co/array.h"
#include "co/err.h"
#include "co/memory.h"
#include "co/bbox.h"

enum {
	X, Y, Z
};

#define T Bbox
struct T {
	real lo[3];
	real hi[3];
};

#define DI(D, d) \
	do { \
		q->lo[D] = array_min(n, d); \
		q->hi[D] = array_max(n, d); \
	} while (0)

int
bbox_ini(T ** pq)
{
	T *q;

	MALLOC(1, &q);
	*pq = q;
	return CO_OK;
}

int
bbox_fin(T * q)
{
	FREE(q);
	return CO_OK;
}

int
bbox_update(T * q, int n, const real * x, const real * y, const real * z)
{
	DI(X, x);
	DI(Y, y);
	DI(Z, z);
	return CO_OK;
}

int
bbox_inside(T * q, real x, real y, real z)
{
#define CM(d, D) (lo[D] < d && d < hi[D])
	real *lo, *hi;

	lo = q->lo;
	hi = q->hi;
	return CM(x, X) && CM(y, Y) && CM(z, Z);
}

int
bbox_lo(T * q, real ** p)
{
	*p = q->lo;
	return CO_OK;
}

int
bbox_hi(T * q, real ** p)
{
	*p = q->hi;
	return CO_OK;
}

real
bbox_xhi(T * q)
{
	return q->hi[X];
}

real
bbox_zhi(T * q)
{
	return q->hi[Z];
}
