#include <stdio.h>
#include <stdlib.h>

#include <real.h>

#include <co/array.h>
#include <co/err.h>
#include <co/memory.h>
#include <co/macro.h>
#include <co/punto.h>

#include <co/cell2.h>

enum {X, Y};
static int n;
static int nx = 20;
static int ny = 20;
real lo[2], hi[2], size;

static int
ini(real *x, real *y)
{
	real x0, y0;
	int i, j, k;
	k = 0;
	for (i = 0; i < nx; i++)
		for (j = 0; j < ny; j++) {
			x0 = lo[X] + (hi[X] - lo[X])*(i + 0.5)/nx;
			y0 = lo[Y] + (hi[Y] - lo[Y])*(j + 0.5)/ny;
			x[k] = x0;
			y[k] = y0;
			k++;
	}
	return CO_OK;
}

static int
euler_step(real dt, int n, const real *vx, const real *vy, real *x, real *y)
{
	int i;
	for (i = 0; i < n; i++) {
		x[i] += dt*vx[i];
		y[i] += dt*vy[i];
	}
	return CO_OK;
}

static int
body_force(int n, const real *x, __UNUSED const real *y, real *fx, __UNUSED real *fy)
{
	int i;
	for (i = 0; i < n; i++)
		fx[i] += y[i] > 0 ? 1 : -1;
	return CO_OK;
}

int
main(void)
{
	static real *x, *y, *vx, *vy, *fx, *fy;
	Cell2 *cell;

	int i, j, k, t, *a;
	real dt;

	lo[X] = -0.5; hi[X] = 0.5;
	lo[Y] = -0.5; hi[Y] =0.5;
	size = 0.2;
	
	n = nx * ny;
	MALLOC(n, &x);
	MALLOC(n, &y);
	CALLOC(n, &vx);
	CALLOC(n, &vy);
	MALLOC(n, &fx);
	MALLOC(n, &fy);
	ini(x, y);
	cell2_pp_ini(lo, hi, size, &cell);

	dt = 0.01;
	for (t = 0; t < 100; t ++) {
		array_zero(n, fx);
		array_zero(n, fy);
		body_force(n, x, y, fx, fy);
		cell2_wrap(cell, n, x, y);
		if (t % 10 == 0) {
			const real *q[] = {x, y, NULL};
			punto_fwrite(n, q, stdout);
			printf("\n\n");
		}

		cell2_push(cell, n, x, y);
		for (i = 0; i < 1; i++) {
			cell2_parts(cell, x[i], y[i], &a);
			while ( (j = *a++) != -1) {
				if (j == i) continue;				
				//printf("%g %g %g %g\n", x[i], y[i], x[j] , y[j]);
			}
		}
		euler_step(dt, n, vx, vy, x, y);
		euler_step(dt, n, fx, fy, vx, vy);
		MSG("vx[0] = %g %g", fx[0], dt);

	}

	cell2_fin(cell);
	FREE(x);
	FREE(y);
	FREE(vx);
	FREE(vy);
	FREE(fx);
	FREE(fy);
}
