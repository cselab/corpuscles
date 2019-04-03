#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>

#include "real.h"
#include "co/err.h"
#include "co/he.h"
#include "co/macro.h"
#include "co/memory.h"

#include "co/list/c.h"
#include "co/list/tri3.h"

#define T Tri3List
#define FMT CO_REAL_OUT

enum {
	X, Y, Z
};

struct T
{
	int ny, nz;
	real lo[3], hi[3], size;
	Clist *clist;
};

#define SIDE(n, D) \
	do { \
		l = hi[D] - lo[D]; \
		n = l / size; \
		if (n * size < l) n++; \
	} while (0)

int
tri3list_ini(const real lo[3], const real hi[3], real size, T **pq)
{
	T *q;
	int nx, ny, nz;
	real l;
	Clist *clist;

	MALLOC(1, &q);

	SIDE(nx, X);
	SIDE(ny, Y);
	SIDE(nz, Z);

	clist_gen_nnn(nx, ny, nz, &clist);

	q->size = size;
	q->lo[X] = lo[X];
	q->lo[Y] = lo[Y];
	q->lo[Z] = lo[Z];

	q->hi[X] = hi[X];
	q->hi[Y] = hi[Y];
	q->hi[Z] = hi[Z];

	q->ny = ny;
	q->nz = nz;
	q->clist = clist;

	*pq = q;
	return CO_OK;
}

int
tri3list_fin(T *q)
{
	clist_fin(q->clist);
	FREE(q);
	return CO_OK;
}

static int
map(T *q, real x, real y, real z, int *i, int *j, int *k)
{
	real *lo, size;

	size = q->size;

	lo = q->lo;

	x -= lo[X];
	y -= lo[Y];
	z -= lo[Z];

	*i = x / size;
	*j = y / size;
	*k = z/ size;
	return CO_OK;
}

#define IDX  i*ny*nz + j*nz + k
int
tri3list_push(T *q, He *he, const real *x, const real *y, const real *z)
{
	int i, j, k, m;
	int ny, nz, n;
	clist_reset(q->clist);
	ny = q->ny;
	nz = q->nz;
	n = he_nv(he);

	for (m = 0; m < n; m++) {
		map(q, x[m], y[m], z[m], &i, &j, &k);
		if (clist_push(q->clist, IDX, m) != CO_OK) {
			MSG("ijk: %d %d %d", i, j, k);
			MSG("nyz: %d %d", ny, nz);
			ERR(CO_INDEX, "fail to push particle: " FMT " " FMT " " FMT, x[m], y[m], z[m]);
		}
	}
	return CO_OK;
}

int
tri3list_parts(T *q, real x, real y, real z)
{
	int i, j, k, ny, nz;
	ny = q->ny;
	nz = q->nz;
	map(q, x, y, z, &i, &j, &k);
/*	if ( clist_parts(q->clist, IDX, a) != CO_OK) {
		MSG("ijk: %d %d %d", i, j, k);
		MSG("nyz: %d %d", ny, nz);
		ERR(CO_INDEX, "clist_parts failed" FMT " " FMT " " FMT, x, y, z);
	}
*/
	return CO_OK;
}
