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
ini(const real lo[3], const real hi[3], real size, int (*gen)(int, int, int, Clist**), T **pq)
{
	T *q;
	int nx, ny, nz;
	real lx, ly, lz;
	Clist *clist;

	MALLOC(1, &q);

	lx = hi[X] - lo[X];
	ly = hi[Y] - lo[Y];
	lz = hi[Z] - lo[Z];

	nx= lx/size;
	if (nx * size < lx) nx++;

	ny = ly/size;
	if (ny * size < ly) ny++;

	nz = lz/size;
	if (nz * size < lz) nz++;

	gen(nx, ny, nz, &clist);

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
	status = ini(lo, hi, size, clist_gen_ppp, pq);
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
map(T *q, real x, real y, real z, int *i, int *j, int *k)
{
	real *lo, *hi, size;

	size = q->size;

	lo = q->lo;
	hi = q->hi;

	x -= lo[X];
	y -= lo[Y];
	z -= lo[Z];

	*i = x / size;
	*j = y / size;
	*k = z/ size;
	return CO_OK;
}

int
cell3_push(T *q, int n, const real *x, const real *y, const real *z)
{
	int i, j, k, m;
	int ny, nz;

	clist_reset(q->clist);
	ny = q->ny;
	nz = q->nz;
	for (m = 0; m < n; m++) {
		map(q, x[m], y[m], z[m], &i, &j, &k);
		clist_push(q->clist, i*ny*nz + j*nz + k, m);
	}
	return CO_OK;
}

int
cell3_parts(T *q, real x, real y, real z, int **a)
{
	int i, j, k, ny, nz;
	ny = q->ny;
	nz = q->nz;
	map(q, x, y, z, &i, &j, &k);
	return clist_parts(q->clist, i*ny*nz + j*nz + k, a);
}

int
cell3_len(T *q, real x, real y, real z)
{
	int i, j, k, ny, nz;
	ny = q->ny;
	nz = q->nz;
	map(q, x, y, z, &i, &j, &k);
	return clist_len(q->clist, i*ny*nz + j*nz + k);
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