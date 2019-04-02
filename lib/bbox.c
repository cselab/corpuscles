#include "real.h"
#include "co/array.h"
#include "co/err.h"
#include "co/bbox.h"

enum {
	X, Y, Z
};
#define T Bbox
#define DI(D, d) \
	do { \
		q->lo[D] = array_min(n, d); \
		q->hi[D] = array_max(n, d); \
	} while (0)

int
bbox_ini(int n, const real *x, const real *y, const real *z, /**/ T *q)
{
	DI(X, x);
	DI(Y, y);
	DI(Z, z);
	return CO_OK;
}

