#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <real.h>

#include <co/array.h>
#include <co/err.h>
#include <co/memory.h>
#include <co/macro.h>
#include <co/punto.h>
#include <co/kernel.h>
#include <co/cell2.h>

#include <alg/rng.h>

#define pi (3.141592653589793)

#define BEGIN \
	for (i = 0; i < n; i++) { \
		xi = x[i]; yi = y[i]; \
		cell2_parts(cell, xi, yi, &a); \
		while ( (j = *a++) != -1) { \
			xj = x[j]; yj = y[j]; \
			cell2_bring(cell, xi, yi, &xj, &yj); \
			xr = xi - xj; \
			yr = yi  - yj; \
			rsq = xr*xr + yr*yr; \
			if (rsq > size*size) continue; \
			r = sqrt(rsq);
#define END } }

enum
{
	X, Y
};
static int n;
#define nx  (20)
#define ny  (20)
static const real size = 5.0/nx;
static const real mass = 1.0/(nx*ny);
static real lo[2], hi[2];
static AlgRng *rng;
static Kernel *kernel;

static int
grid(real *x, real *y)
{
	real x0, y0, dx,a, b;
	int i, j, k;
	k = 0;
	dx = (hi[X] - lo[X])/nx;
	a = -0.1*dx;
	b =   0.1*dx;
	for (i = 0; i < nx; i++)
		for (j = 0; j < ny; j++) {
			x0 = lo[X] + (hi[X] - lo[X])*(i + 0.5)/nx;
			y0 = lo[Y] + (hi[Y] - lo[Y])*(j + 0.5)/ny;
			x[k] = x0 + alg_rng_uniform(rng, a, b);
			y[k] = y0 + alg_rng_uniform(rng, a, b);
			k++;
		}
	return CO_OK;
}

static int
ini(real *x, real *y)
{
	return grid(x, y);
}

int
main(void)
{
	real *x, *y, *rho, *color;
	Cell2 *cell;

	int i0, i, j, *a;
	real xi, yi, xj, yj, xr, yr, rsq, r, w;

	alg_rng_ini(&rng);
	kernel_ini(KERNEL_2D, KERNEL_YANG, &kernel);

	lo[X] = -0.5;
	hi[X] = 0.5;
	lo[Y] = -0.5;
	hi[Y] =0.5;
	n = nx * ny;

	MALLOC(n, &x);
	MALLOC(n, &y);
	MALLOC(n, &rho);
	CALLOC(n, &color);
	ini(x, y);
	cell2_pp_ini(lo, hi, size, &cell);
	array_zero(n, rho);
	cell2_push(cell, n, x, y);
	i0 = 4;
	BEGIN {
		w = kernel_w(kernel, size, r);
		rho[i] += mass*w;
		if (i == i0) color[j] = 1;
	} END
	color[i0] = 2;

	MSG("rho[%d]: %g", i0, rho[i0]);
	MSG("size: %g", size);
	const real *q[] = {x, y, color, rho, NULL};
	punto_fwrite(n, q, stdout);

	cell2_fin(cell);
	FREE(x);
	FREE(y);
	FREE(rho);
	kernel_fin(kernel);
	alg_rng_fin(rng);
}
