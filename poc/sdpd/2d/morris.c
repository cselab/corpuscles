#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>

#include <real.h>

#include <co/array.h>
#include <co/err.h>
#include <co/memory.h>
#include <co/macro.h>
#include <co/punto.h>
#include </u/co/lib/co/kernel.h>
#include <co/cell2.h>

#include <alg/rng.h>

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

enum {
	X, Y};
static int n;
#define nx  (20)
#define ny  (20)
static const real c = 10.0;
static const real mu = 1.0;
static const real size = 2.0/nx;
static const real g[] = {5, 0};
static const real mass = 1.0/(nx*ny);
static const real R = 0.2;
static const real dt = 0.00025;
static real lo[2], hi[2];
static AlgRng *rng;
static Kernel *kernel;
static real *x, *y, *vx, *vy, *fx, *fy, *rho, *p;
static Cell2 *cell;

static int
grid(real *x, real *y)
{
	real x0, y0, dx,a, b;
	int i, j, k;
	k = 0;
	dx = (hi[X] - lo[X])/nx;
	a = -0.2*dx;
	b =   0.2*dx;
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
rnd(real *x, real *y)
{
	int i, k;
	for (i = k = 0; i < n; i++) {
		x[k] = alg_rng_uniform(rng, lo[X], hi[X]);
		y[k] = alg_rng_uniform(rng, lo[Y], hi[Y]);
		k++;
	}
	return CO_OK;
}

static int
ini(real *x, real *y)
{
	return rnd(x, y);
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
euler_step_fun(real dt, int (*f)(real, real), int n, const real *vx, const real *vy, real *xx, real *yy)
{
	int i;
	for (i = 0; i < n; i++) {
		if (f(x[i], y[i])) continue;
		xx[i] += dt*vx[i];
		yy[i] += dt*vy[i];
	}
	return CO_OK;
}

static int
circle(real x, real y)
{
	return x*x + y*y < R*R;
}

static int
body_force(int n, __UNUSED const real *x, __UNUSED const real *y, real *fx,  real *fy)
{
	int i;
	for (i = 0; i < n; i++) {
		fx[i] += g[X];
		fy[i] += g[Y];
	}
	return CO_OK;
}

static real
eq_state(real rho)
{
	return c*c*rho;
}

int
force(void)
{
	int i, j, k, t, *a;
	real xi, yi, xj, yj, xr, yr, rsq, r, w, dw;
	real coeff;

	array_zero(n, fx);
	array_zero(n, fy);
	array_zero(n, rho);
	body_force(n, x, y, fx, fy);
	cell2_wrap(cell, n, x, y);
	cell2_push(cell, n, x, y);
	BEGIN {
		w = kernel_w(kernel, size, r);
		rho[i] += mass*w;
	} END
	
	 for (i = 0; i < n; i++)
		p[i] = eq_state(rho[i]);

	BEGIN {
		if (j == i) continue;
		dw = kernel_dw(kernel, size, r);
		coeff = p[i]/(rho[i]*rho[i]) + p[j]/(rho[j]*rho[j]);
		coeff *= mass*dw/r;
		fx[i]  -= coeff * (xi - xj);
		fy[i]  -= coeff * (yi - yj);

		coeff = 1/(rho[i]*rho[j]);
		coeff *= 2*mass*mu*dw/r;
		fx[i]  += coeff * (vx[i] - vx[j]);
		fy[i]  += coeff * (vy[i] - vy[j]);
	} END	
	return CO_OK;
}

int
dump(int t)
{
	static int First = 1;
	if (t % 100 == 0) {
		if (First) First = 0;
		else printf("\n");
		const real *q[] = {
			x, y, vx, vy, rho, NULL
		};
		punto_fwrite(n, q, stdout);
		MSG("rho[0] = %g", rho[0]);
	}
	return CO_OK;
}

int
main(void)
{
	int t;

	alg_rng_ini(&rng);
	kernel_ini(KERNEL_2D, KERNEL_YANG, &kernel);

	lo[X] = -0.5; 
	hi[X] = 0.5;
	lo[Y] = -0.5; 
	hi[Y] =0.5;
	n = nx * ny;

	MALLOC(n, &x);
	MALLOC(n, &y);
	CALLOC(n, &vx);
	CALLOC(n, &vy);
	MALLOC(n, &fx);
	MALLOC(n, &fy);
	MALLOC(n, &rho);
	MALLOC(n, &p);
	ini(x, y);
	cell2_pp_ini(lo, hi, size, &cell);

	for (t = 0; t < 1000; t ++) {
		force();
		euler_step(dt,  n, vx, vy, x, y);
		euler_step(dt,  n, fx, fy, vx, vy);
		dump(t);
	}

	array_zero(n, vx);
	array_zero(n, vy);
	for (/**/; t < 20000; t ++) {
		force();
		euler_step_fun(dt, circle, n, vx, vy, x, y);
		euler_step_fun(dt, circle, n, fx, fy, vx, vy);
		dump(t);
	}

	cell2_fin(cell);
	FREE(x);
	FREE(y);
	FREE(vx);
	FREE(vy);
	FREE(fx);
	FREE(fy);
	FREE(rho);
	FREE(p);
	kernel_fin(kernel);
	alg_rng_fin(rng);
}
