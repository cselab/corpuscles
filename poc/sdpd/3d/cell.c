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
#include <co/cell3.h>

#include <alg/rng.h>

#define pi (3.141592653589793)
#define FMT CO_REAL_OUT

#define BEGIN \
	for (i = 0; i < n; i++) { \
		xi = x[i]; yi = y[i]; zi = z[i]; \
		cell3_parts(cell, xi, yi, zi, &a); \
		while ( (j = *a++) != -1) { \
			xj = x[j]; yj = y[j]; zj = z[j]; \
			cell3_bring(cell, xi, yi, zi, &xj, &yj, &zj); \
			xr = xi - xj; \
			yr = yi  - yj; \
			zr = zi - zj; \
			rsq = xr*xr + yr*yr + zr*zr; \
			if (rsq > size*size) continue; \
			r = sqrt(rsq);
#define END } }

enum
{
	X, Y, Z
};
static int n;
#define nx (10)
#define ny (20)
#define nz (30)
static const real lo[3] = {-0.25, -0.5, -1};
static const real hi[3] = {0.25, 0.5, 1};
static real mass, size;
static AlgRng *rng;
static Kernel *kernel;

static int
grid(real *x, real *y, real *z)
{
	real x0, y0, z0, dx,a, b;
	int i, j, k, m;
	dx = (hi[X] - lo[X])/nx;
	a = -0.05*dx;
	b =   0.05*dx;
	for (i = m = 0; i < nx; i++)
		for (j = 0; j < ny; j++)
			for (k = 0; k < nz; k++) {
				x0 = lo[X] + (hi[X] - lo[X])*(i + 0.5)/nx;
				y0 = lo[Y] + (hi[Y] - lo[Y])*(j + 0.5)/ny;
				z0 = lo[Z] + (hi[Z] - lo[Z])*(k + 0.5)/nz;
				x[m] = x0 + alg_rng_uniform(rng, a, b);
				y[m] = y0 + alg_rng_uniform(rng, a, b);
				z[m] = z0 + alg_rng_uniform(rng, a, b);
				m++;
			}
	return CO_OK;
}

static int
ini(real *x, real *y, real *z)
{
	return grid(x, y, z);
}

int
main(void)
{
	real *x, *y, *z, *rho, *color;
	Cell3 *cell;
	int i0, i, j, *a;
	real V, xi, yi, zi, xj, yj, zj, xr, yr, zr, rsq, r, w;

	err_set_ignore();

	alg_rng_ini(&rng);
	kernel_ini(KERNEL_3D, KERNEL_YANG, &kernel);
	n = nx*ny*nz;
	V = (hi[X] - lo[X])*(hi[Y] - lo[Y])*(hi[Z] - lo[Z]);
	mass = V/n;
	size = 2 * (hi[X] - lo[X]) / nx;

	MALLOC(n, &x);
	MALLOC(n, &y);
	MALLOC(n, &z);
	MALLOC(n, &rho);
	CALLOC(n, &color);
	ini(x, y, z);
	cell3_ppp_ini(lo, hi, size, &cell);
	array_zero(n, rho);
	cell3_push(cell, n, x, y, z);

	i0 = 500;
	BEGIN {
		w = kernel_w(kernel, size, r);
		rho[i] += mass*w;
		if (i == i0) color[j] = 1;
	} END
	color[i0] = 2;

	MSG("rho[%d]: %g", i0, rho[i0]);
	MSG("size: %g", size);
	const real *q[] = {x, y, z, color, rho, NULL};
	punto_fwrite(n, q, stdout);
	fprintf(stderr, "rho: " FMT " " FMT " " FMT "\n", array_min(n, rho), array_mean(n, rho), array_max(n, rho));

	cell3_fin(cell);
	FREE(x);
	FREE(y);
	FREE(z);
	FREE(rho);
	kernel_fin(kernel);
	alg_rng_fin(rng);
}

/* 
	Put
	make
	./cell > q
	punto -c q

*/