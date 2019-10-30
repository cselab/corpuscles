#include <stdio.h>
#include "real.h"
#include "co/array.h"
#include "co/err.h"
#include "co/memory.h"
#include "co/bbox2.h"

enum {
  X, Y
};

#define T Bbox2
struct T {
  real lo[2];
  real hi[2];
};

#define DI(D, d) \
	do { \
		q->lo[D] = array_min(n, d); \
		q->hi[D] = array_max(n, d); \
	} while (0)

int
bbox2_ini(T ** pq)
{
  T *q;

  MALLOC(1, &q);
  *pq = q;
  return CO_OK;
}

int
bbox2_fin(T * q)
{
  FREE(q);
  return CO_OK;
}

int
bbox2_update(T * q, int n, const real * x, const real * y)
{
  DI(X, x);
  DI(Y, y);
  return CO_OK;
}

int
bbox2_inside(T * q, real x, real y)
{
#define CM(d, D) (lo[D] < d && d < hi[D])
  real *lo, *hi;

  lo = q->lo;
  hi = q->hi;
  return CM(x, X) && CM(y, Y);
}

int
bbox2_lo(T * q, real ** p)
{
  *p = q->lo;
  return CO_OK;
}

int
bbox2_hi(T * q, real ** p)
{
  *p = q->hi;
  return CO_OK;
}

real
bbox2_xhi(T * q)
{
  return q->hi[X];
}

real
bbox2_yhi(T * q)
{
  return q->hi[Y];
}
