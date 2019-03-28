#include <stdio.h>
#include <stdlib.h>

#include <real.h>
#include <co/err.h>
#include <co/memory.h>

#include <co/list/c.h>

#define T Cell2
typedef struct Cell2 Cell2;
int cell2_pp_ini(const real lo[], const real hi[], real size, T**);
int cell2_push(T*, int, const real[], const real[]);
int cell2_parts(T*, real, real, int**);
int cell2_len(T*, real, real);
int cell2_reset(T*);
int cell2_fin(T*);

struct T
{
	int size,  ny;
	real lo[2], hi[3];
	Clist *clist;
};

int cell2_pp_ini(const real lo[2], const real hi[3], real size, T **pq)
{
	enum {X, Y};
	int nx, ny;
	T *q;
	real lx, ly;
	Clist *clist;

	MALLOC(1, &q);

	lx = hi[X] - lo[X];
	ly = hi[Y] - lo[Y];

	if (lx < 0)
		ERR(CO_INDEX, "lx < 0");
	if (ly < 0)
		ERR(CO_INDEX, "ly < 0");

	nx= lx/size;
	if (nx * size < lx) nx++;
	
	ny = ly/size;
	if (ny * size < ly) ny++;

	clist_gen_pp(nx, ny, &clist);

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

int cell2_fin(T *q)
{
	clist_fin(q->clist);
	FREE(q);
	return CO_OK;
}

static int map(T *q, real x, real y, int *i, int *j)
{
	enum {X, Y};
	real *lo, *hi;
	lo = q->lo;
	hi = q->hi;
	
	x -= lo[X];
	y -= lo[Y];

	*i = x / (hi[X] - lo[X]);
	*j = y / (hi[Y] - lo[Y]);
	return CO_OK;
}

int cell2_push(T *q, int n, const real *x, const real *y)
{
	int i, j, k;
	int ny;

	ny = q->ny;
	for (k = 0; k < n; k++) {
		map(q, x[k], y[k], &i, &j);
		clist_push(q->clist, i*ny + j, k);
	}
	return CO_OK;
}

int cell2_parts(T *q, real x, real y, int **a)
{
	int i, j, ny;
	ny = q->ny;
	map(q, x, y, &i, &j);
	return clist_parts(q->clist, i*ny + j, a);
}

int main(void)
{
	enum {X, Y};
	Cell2 *cell;
	int nx, ny, n;
	real lo[2], hi[2], size;
	real *x, *y, x0, y0;
	int i, j, k, *a;

	lo[X] = lo[Y] = 0;
	hi[X] = hi[Y] = 1;
	size = 0.2;

	nx = ny = 5;
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
 
	for (i = 0; i < n; i++) {
		cell2_parts(cell, x[i], y[i], &a);
		for (j = 0; a[j] !=-1; j++) {
			if (j == i) continue;
			printf("%d %d\n", i, j);
		}
	}

	cell2_fin(cell);
}
