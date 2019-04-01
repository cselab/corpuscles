#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>

#include "real.h"
#include "co/err.h"
#include "co/memory.h"

#include "co/list/c.h"
#include "co/cell3.h"

#define T Cell3

enum {
	X, Y, Z
};
struct T
{
	int ny, nz;
	real lo[3], hi[3], size;
	Clist *clist;
	int (*wrp)(T*q, real*, real*, real*);
	int (*brn)(T* q, real, real, real, real*, real*, real*);
};

static int
ini(const real lo[3], const real hi[3], real size, int (*gen)(int, int, Clist**), T **pq)
{
	int nx, ny;
	T *q;
	real lx, ly;
	Clist *clist;

	MALLOC(1, &q);

	lx = hi[X] - lo[X];
	ly = hi[Y] - lo[Y];

	nx= lx/size;
	if (nx * size < lx) nx++;

	ny = ly/size;
	if (ny * size < ly) ny++;

	gen(nx, ny, &clist);

	q->size = size;
	q->lo[X] = lo[X];
	q->lo[Y] = lo[Y];

	q->hi[X] = hi[X];
	q->hi[Y] = hi[Y];

	q->ny = ny;
	q->clist = clist;

	*pq = q;
	return CO_OK;
}

static
int wrp_ppp(T *q, real *x, real *y, real *z)
{
	real *lo, *hi;

	lo = q->lo;
	hi = q->hi;
	if (*x > hi[X]) *x -= (hi[X] - lo[X]);
	if (*x < lo[X]) *x += (hi[X] - lo[X]);
	if (*y > hi[Y]) *y -= (hi[Y] - lo[Y]);
	if (*y < lo[Y]) *y += (hi[Y] - lo[Y]);
	if (*z > hi[Z]) *z -= (hi[Z] - lo[Z]);
	if (*z < lo[Z]) *z += (hi[Z] - lo[Z]);
	return CO_OK;
}


#define BRN(a, b, l) \
	do { \
		if (fabs(*a - b) > fabs(*a + l - b)) *a += l; \
		else if (fabs(*a - b) > fabs(*a - l - b)) *a -= l; \
	} while (0)
static
int brn_ppp(T *q, real x, real y, real z, real *u, real *v, real *w)
{
	real lx, ly, lz;
	lx = q->hi[X] - q->lo[X];
	ly = q->hi[Y] - q->lo[Y];
	lz = q->hi[Z] - q->lo[Z];
	BRN(u, x, lx);
	BRN(v, y, ly);
	BRN(w, z, lz);
	return CO_OK;
}

int
cell3_ppp_ini(const real lo[3], const real hi[3], real size, T **pq)
{
	int status;
	status = ini(lo, hi, size, clist_gen_pp, pq);
	(*pq)->wrp = wrp_ppp;
	(*pq)->brn = brn_ppp;
	return status;
}

int
cell3_fin(T *q)
{
	clist_fin(q->clist);
	FREE(q);
	return CO_OK;
}

static int
map(T *q, real x, real y, int *i, int *j)
{
	real *lo, *hi, size;

	size = q->size;

	lo = q->lo;
	hi = q->hi;

	x -= lo[X];
	y -= lo[Y];

	*i = x / size;
	*j = y / size;
	return CO_OK;
}

int
cell3_push(T *q, int n, const real *x, const real *y, const real *z)
{
	int i, j, k;
	int ny;

	clist_reset(q->clist);
	ny = q->ny;
	for (k = 0; k < n; k++) {
		map(q, x[k], y[k], &i, &j);
		clist_push(q->clist, i*ny + j, k);
	}
	return CO_OK;
}

int
cell3_parts(T *q, real x, real y, real z, int **a)
{
	int i, j, ny;
	ny = q->ny;
	map(q, x, y, &i, &j);
	return clist_parts(q->clist, i*ny + j, a);
}

int
cell3_len(T *q, real x, real y, real z)
{
	int i, j, ny;
	ny = q->ny;
	map(q, x, y, &i, &j);
	return clist_len(q->clist, i*ny + j);
}

int
cell3_wrap(T *q, int n, real *x, real *y, real *z)
{
	int i;
	if (q->wrp == NULL)
		return CO_OK;
	for (i = 0; i < n; i++)
		q->wrp(q, &x[i], &y[i], &z[i]);
	return CO_OK;
}

int
cell3_bring(T *q, real x, real y, real z, real *u, real *v, real *w)
{
	if (q->brn == NULL)
		return CO_OK;

	return q->brn(q, x, y, z, u, v, w);
}