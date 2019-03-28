#include <stdio.h>
#include <stdlib.h>

#include <real.h>
#include <co/err.h>
#include <co/memory.h>

#include <co/list/c.h>

#define T Cell2
typedef struct Cell2 Cell2;
int cell2_pp_ini(const real lo[], const real hi[], real size, T**);
int cell2_push(T*, const real[], const real[], int);
int cell2_head(T*, real,  real, int**);
int cell2_len(T*, real, real);
int cell2_reset(T*);
int cell2_fin(T*);

struct T
{
	int size;
	real lo[2], hi[3];
};

int cell2_pp_ini(const real lo[2], const real hi[3], real size, T **pq)
{
	enum {X, Y};
	int x, y;
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

	x= lx/size;
	if (x * size < lx) x++;
	
	y = ly/size;
	if (y * size < ly) y++;

	clist_gen_pp(x, y, &clist);

	q->size = size;
	q->lo[X] = lo[X];
	q->lo[Y] = lo[Y];

	q->hi[X] = hi[X];
	q->hi[Y] = hi[Y];

	*pq = q;
	return CO_OK;
}

int cell2_fin(T *q)
{
	FREE(q);
	return CO_OK;
}

int main(void)
{
	enum {X, Y};
	Cell2 *cell;
	int nx, ny, n;
	real lo[2], hi[2], size;
	real *x, *y, x0, y0;

	lo[X] = lo[Y] = 0;
	hi[X] = hi[Y] = 1;
	size = 0.2;

	nx = ny = 5;
	n = nx * ny;
	MALLOC(n, &x);
	MALLOC(n, &y);

	k = 0;
	for (i = 0; i < nx; i++)
		for (j = 0; j < ny; j++) {
			x0 = lo[X] + (hi[X] - lo[X])*(i + 0.5)/nx;
			y0 = lo[Y] + (hi[Y] - lo[Y])*(j + 0.5)/ny;
			x[k] = x0;
			y[k] = y0;
			k++;
	}

	cell2_pp_ini(lo, hi, size, &cell);
	cell2_fin(cell);
}
