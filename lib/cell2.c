#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>

#include "real.h"
#include "co/err.h"
#include "co/macro.h"
#include "co/memory.h"

#include "co/list/c.h"
#include "co/cell2.h"

#define T Cell2
#define FMT CO_REAL_OUT

enum {
	X, Y
};
struct T {
	int ny;
	real lo[2], hi[2], size;
	Clist *clist;
	int (*wrp)(T * q, real *, real *);
	int (*brn)(T * q, real, real, real *, real *);
};

static int
ini(const real lo[2], const real hi[2], real size,
	int (*gen)(int, int, Clist **), T ** pq)
{
	int nx, ny;
	T *q;
	real lx, ly;
	Clist *clist;

	MALLOC(1, &q);

	lx = hi[X] - lo[X];
	ly = hi[Y] - lo[Y];

	nx = lx / size;
	if (nx * size < lx)
		nx++;

	ny = ly / size;
	if (ny * size < ly)
		ny++;

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
	int
wrp_pp(T * q, real * x, real * y)
{
	real *lo, *hi;

	lo = q->lo;
	hi = q->hi;
	if (*x > hi[X])
		*x -= (hi[X] - lo[X]);
	if (*x < lo[X])
		*x += (hi[X] - lo[X]);
	if (*y > hi[Y])
		*y -= (hi[Y] - lo[Y]);
	if (*y < lo[Y])
		*y += (hi[Y] - lo[Y]);
	return CO_OK;
}

static
	int
wrp_pn(T * q, real * x, __UNUSED real * y)
{
	real *lo, *hi;

	lo = q->lo;
	hi = q->hi;
	if (*x > hi[X])
		*x -= (hi[X] - lo[X]);
	if (*x < lo[X])
		*x += (hi[X] - lo[X]);
	return CO_OK;
}

static
	int
wrp_np(T * q, __UNUSED real * x, real * y)
{
	real *lo, *hi;

	lo = q->lo;
	hi = q->hi;
	if (*y > hi[Y])
		*y -= (hi[Y] - lo[Y]);
	if (*y < lo[Y])
		*y += (hi[Y] - lo[Y]);
	return CO_OK;
}

#define BRN(a, b, l) \
	do { \
		if (fabs(*a - b) > fabs(*a + l - b)) *a += l; \
		else if (fabs(*a - b) > fabs(*a - l - b)) *a -= l; \
	} while (0)
static
	int
brn_pp(T * q, real x, real y, real * u, real * v)
{
	real lx, ly;

	lx = q->hi[X] - q->lo[X];
	ly = q->hi[Y] - q->lo[Y];
	BRN(u, x, lx);
	BRN(v, y, ly);
	return CO_OK;
}

static
	int
brn_pn(T * q, real x, __UNUSED real y, real * u, __UNUSED real * v)
{
	real lx;

	lx = q->hi[X] - q->lo[X];
	BRN(u, x, lx);
	return CO_OK;
}

static
	int
brn_np(T * q, __UNUSED real x, real y, __UNUSED real * u, real * v)
{
	real ly;

	ly = q->hi[Y] - q->lo[Y];
	BRN(v, y, ly);
	return CO_OK;
}

int
cell2_pp_ini(const real lo[2], const real hi[2], real size, T ** pq)
{
	int status;

	status = ini(lo, hi, size, clist_gen_pp, pq);
	(*pq)->wrp = wrp_pp;
	(*pq)->brn = brn_pp;
	return status;
}

int
cell2_np_ini(const real lo[2], const real hi[2], real size, T ** pq)
{
	int status;

	status = ini(lo, hi, size, clist_gen_np, pq);
	(*pq)->wrp = wrp_np;
	(*pq)->brn = brn_np;
	return status;
}

int
cell2_pn_ini(const real lo[2], const real hi[2], real size, T ** pq)
{
	int status;

	status = ini(lo, hi, size, clist_gen_pn, pq);
	(*pq)->wrp = wrp_pn;
	(*pq)->brn = brn_pn;
	return status;
}

int
cell2_nn_ini(const real lo[2], const real hi[2], real size, T ** pq)
{
	int status;

	status = ini(lo, hi, size, clist_gen_nn, pq);
	(*pq)->wrp = NULL;
	(*pq)->brn = NULL;
	return status;
}

int
cell2_fin(T * q)
{
	clist_fin(q->clist);
	FREE(q);
	return CO_OK;
}

static int
map(T * q, real x, real y, int *i, int *j)
{
	real *lo, size;

	size = q->size;

	lo = q->lo;
	x -= lo[X];
	y -= lo[Y];
	*i = x / size;
	*j = y / size;
	return CO_OK;
}

int
cell2_push(T * q, int n, const real * x, const real * y)
{
	int i, j, k;
	int ny;

	clist_reset(q->clist);
	ny = q->ny;
	for (k = 0; k < n; k++) {
		map(q, x[k], y[k], &i, &j);
		if (clist_push(q->clist, i * ny + j, k) != CO_OK) {
			MSG("ijk: %d %d", i, j);
			MSG("ny: %d", ny);
			ERR(CO_INDEX, "fail to push particle: " FMT " " FMT, x[k],
				y[k]);
		}
	}
	return CO_OK;
}

int
cell2_parts(T * q, real x, real y, int **a)
{
	int i, j, ny;

	ny = q->ny;
	map(q, x, y, &i, &j);
	if (clist_parts(q->clist, i * ny + j, a) != CO_OK) {
		MSG("ijk: %d %d", i, j);
		MSG("ny: %d", ny);
		ERR(CO_INDEX, "clist_parts failed" FMT " " FMT, x, y);
	}
	return CO_OK;
}

int
cell2_len(T * q, real x, real y)
{
	int i, j, ny;

	ny = q->ny;
	map(q, x, y, &i, &j);
	return clist_len(q->clist, i * ny + j);
}

int
cell2_wrap(T * q, int n, real * x, real * y)
{
	int i;

	if (q->wrp == NULL)
		return CO_OK;
	for (i = 0; i < n; i++)
		q->wrp(q, &x[i], &y[i]);
	return CO_OK;
}

int
cell2_bring(T * q, real x, real y, real * u, real * v)
{
	if (q->brn == NULL)
		return CO_OK;

	return q->brn(q, x, y, u, v);
}
