#include <stdio.h>
#include <stdlib.h>

#include <real.h>
#include <co/err.h>
#include <co/memory.h>

#include <co/list/c.h>

#define T Cell2
typedef struct Cell2 Cell2;
int cell2_pp_ini(const real lo2[], const real hi[2], real size, T**);
int cell2_push(T*, int, const real*, const real*);
int cell2_parts(T*, real, real, int**);
int cell2_len(T*, real, real);
int cell2_wrap(T*, int, real*, real*);
int cell2_fin(T*);

enum {X, Y};
struct T
{
	int ny;
	real lo[2], hi[3], size;
	Clist *clist;
	int (*wrp)(T*q, real*, real*);
};

static int
ini(const real lo[2], const real hi[3], real size, int (*gen)(int, int, Clist**), T **pq)
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
int wrp_pp(T *q, real *x, real *y)
{
	real *lo, *hi;
	lo = q->lo;
	hi = q->hi;
	if (*x > hi[X]) *x -= (hi[X] - lo[X]);
	if (*x < lo[X]) *x += (hi[X] - lo[X]);

	if (*y > hi[Y]) *y -= (hi[Y] - lo[Y]);
	if (*y < lo[Y]) *y += (hi[Y] - lo[Y]);
	return CO_OK;
}

int
cell2_pp_ini(const real lo[2], const real hi[3], real size, T **pq)
{
	int status;
	status = ini(lo, hi, size, clist_gen_pp, pq);
	(*pq)->wrp = wrp_pp;
	return status;
}

int
cell2_nn_ini(const real lo[2], const real hi[3], real size, T **pq)
{
	int status;
	status = ini(lo, hi, size, clist_gen_nn, pq);
	(*pq)->wrp = NULL;
	return status;
}

int
cell2_fin(T *q)
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
cell2_push(T *q, int n, const real *x, const real *y)
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
cell2_parts(T *q, real x, real y, int **a)
{
	int i, j, ny;
	ny = q->ny;
	map(q, x, y, &i, &j);
	return clist_parts(q->clist, i*ny + j, a);
}

int
cell2_len(T *q, real x, real y)
{
	int i, j, ny;
	ny = q->ny;
	map(q, x, y, &i, &j);
	return clist_len(q->clist, i*ny + j);
}

int
cell2_wrap(T *q, int n, real *x, real *y)
{
	int i;
	if (q->wrp == NULL)
		return CO_OK;
	for (i = 0; i < n; i++)
		q->wrp(q, &x[i], &y[i]);
	return CO_OK;
}

int
main(void)
{
	enum {X, Y};
	Cell2 *cell;
	int nx, ny, n;
	real lo[2], hi[2], size;
	real *x, *y, x0, y0;
	int i, j, k, *a;

	lo[X] = lo[Y] = -0.5;
	hi[X] = hi[Y] = 0.5;
	size = 0.2;

	nx = ny = 10;
	n = nx * ny;
	MALLOC(n, &x);
	MALLOC(n, &y);
	cell2_pp_ini(lo, hi, size, &cell);

	k = 0;
	for (i = 0; i < nx; i++)
		for (j = 0; j < ny; j++) {
			x0 = lo[X] + (hi[X] - lo[X])*(i + 0.5)/nx;
			y0 = lo[Y] + (hi[Y] - lo[Y])*(j + 0.5)/ny;
			x[k] = x0;
			y[k] = y0;
			k++;
	}
	cell2_push(cell, n, x, y);
	cell2_push(cell, n, x, y);

	for (i = 0; i < 2; i++) {
		cell2_parts(cell, x[i], y[i], &a);
		while ( (j = *a++) != -1) {
			if (j == i) continue;
			printf("%g %g %g %g\n", x[i], y[i], x[j]  - x[i], y[j] - y[i]);
		}
	}
	cell2_fin(cell);

	FREE(x);
	FREE(y);
}
