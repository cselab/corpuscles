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
#define nx  (20)
#define ny  (20)
#define nz  (10)
static const real c = 10;
static const real mu = 1;
static const real g[3] = {1, 0, 0};
static const real R = 0.2;
static const real dt = 0.00025;
static real lo[3] = {-1, -1, -0.5};
static real hi[3] = {1, 1, 0.5};
static real mass, size;
static AlgRng *rng;
static Kernel *kernel;
static real *x, *y, *z, *vx, *vy, *vz, *fx, *fy, *fz, *rho, *p;
static Cell3 *cell;

static int
rnd(real *x, real *y, real *z)
{
	int i, k;
	for (i = k = 0; i < n; i++) {
		x[k] = alg_rng_uniform(rng, lo[X], hi[X]);
		y[k] = alg_rng_uniform(rng, lo[Y], hi[Y]);
		z[k]  = alg_rng_uniform(rng, lo[Z], hi[Z]);
		k++;
	}
	return CO_OK;
}

static int
ini(real *x, real *y, real *z)
{
	return rnd(x, y, z);
}


static int
euler_step(real dt, int n, const real *vx, const real *vy, const real*vz, real *x, real *y, real *z)
{
	int i;
	for (i = 0; i < n; i++) {
		x[i] += dt*vx[i];
		y[i] += dt*vy[i];
		z[i] += dt*vz[i];
	}
	return CO_OK;
}

static int
euler_step_fun(real dt, int (*f)(real, real, real), int n, const real *vx, const real *vy, const real *vz, real *xx, real *yy, real *zz)
{
	int i;
	for (i = 0; i < n; i++) {
		if (f(x[i], y[i], z[i])) continue;
		xx[i] += dt*vx[i];
		yy[i] += dt*vy[i];
		zz[i] += dt*vz[i];
	}
	return CO_OK;
}

static int
circle(real x, real y, __UNUSED real z)
{
	return x*x + y*y < R*R;
}

static int
body_force(int n,  __UNUSED const real *x, __UNUSED const real *y, __UNUSED const real *z, real *fx,  real *fy, real *fz)
{
	int i;
	for (i = 0; i < n; i++) {
		fx[i] += g[X];
		fy[i] += g[Y];
		fz[i] += g[Z];
	}
	return CO_OK;
}

static real
eq_state(real rho)
{
	return c*c*rho;
}

static int
force(void)
{
	int i, j, *a;
	real xi, yi, zi, xj, yj, zj, xr, yr, zr, rsq, r, w, dw, coeff;

	array_zero3(n, fx, fy, fz);
	array_zero(n, rho);
	body_force(n, x, y, z, fx, fy, fz);
	cell3_wrap(cell, n, x, y, z);
	cell3_push(cell, n, x, y, z);
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
		fz[i] -= coeff * (zi - zj);

		coeff = 1/(rho[i]*rho[j]);
		coeff *= 2*mass*mu*dw/r;
		fx[i]  += coeff * (vx[i] - vx[j]);
		fy[i]  += coeff * (vy[i] - vy[j]);
		fz[i] += coeff * (vz[i] - vz[j]);
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
			x, y, z, vx, vy, vz, rho, NULL
		};
		punto_fwrite(n, q, stdout);
		MSG("rho: " FMT " " FMT " " FMT, array_min(n, rho), array_mean(n, rho), array_max(n, rho));
	}
	return CO_OK;
}

int
main(void)
{
	real V;
	int t;

	alg_rng_ini(&rng);
	kernel_ini(KERNEL_3D, KERNEL_YANG, &kernel);
	n = nx*ny*nz;
	V = (hi[X] - lo[X])*(hi[Y] - lo[Y])*(hi[Z] - lo[Z]);
	mass = V/n;
	size = 2.5 * (hi[X] - lo[X]) / nx;

	MALLOC(n, &x);
	MALLOC(n, &y);
	MALLOC(n, &z);
	CALLOC(n, &vx);
	CALLOC(n, &vy);
	CALLOC(n, &vz);
	MALLOC(n, &fx);
	MALLOC(n, &fy);
	MALLOC(n, &fz);
	MALLOC(n, &rho);
	MALLOC(n, &p);
	ini(x, y, z);
	cell3_ppp_ini(lo, hi, size, &cell);
	for (t = 0; t < 2000; t++) {   
		force();
		euler_step(dt,  n, vx, vy, vz, x, y, z);
		euler_step(dt,  n, fx, fy, fz, vx, vy, vz);		
		dump(t);
	}

	array_zero3(n, vx, vy, vz);
	array_zero3(n, fx, fy, fz);
	for (/**/; t < 8000; t++) {
		force();
		euler_step_fun(dt, circle, n, vx, vy, vz, x, y, z);
		euler_step_fun(dt, circle, n, fx, fy, fz, vx, vy, vz);
		dump(t);
	}

	cell3_fin(cell);
	FREE(x);
	FREE(y);
	FREE(z);
	FREE(vx);
	FREE(vy);
	FREE(vz);
	FREE(fx);
	FREE(fy);
	FREE(fz);
	FREE(rho);
	FREE(p);
	kernel_fin(kernel);
	alg_rng_fin(rng);
	MSG("end");
}


/*

pkill morris
CFLAGS=-O3 make 
./morris > q
punto -c -G 0:0.3  q

*/
