#include <stdio.h>
#include <stdlib.h>

#include <real.h>
#include <co/err.h>
#include <co/memory.h>

#include <co/cell2.h>

int
main(void)
{
	enum {X, Y};
	Cell2 *cell;
	int nx, ny, n;
	real lo[2], hi[2], size;
	real *x, *y, x0, y0;
	int i, j, k, *a;

	lo[X] = -0.5; lo[Y] = 1.0;
	hi[X] = 2.0; hi[Y] =2.0;
	size = 0.2;

	nx = ny = 20;
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

	for (i = 0; i < 1; i++) {
		cell2_parts(cell, x[i], y[i], &a);
		while ( (j = *a++) != -1) {
			if (j == i) continue;
			printf("%g %g %g %g\n", x[i], y[i], x[j] , y[j]);
		}
	}
	cell2_fin(cell);

	FREE(x);
	FREE(y);
}
