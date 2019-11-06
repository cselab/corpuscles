#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <real.h>

#include <co/array.h>
#include <co/cell3.h>
#include <co/edg.h>
#include <co/err.h>
#include <co/he.h>
#include <co/kernel.h>
#include <co/list/tri3.h>
#include <co/macro.h>
#include <co/memory.h>
#include <co/pre/cons.h>
#include <co/pre/density.h>
#include <co/pre/visc.h>
#include <co/punto.h>
#include <co/surface.h>
#include <co/tri.h>
#include <co/vec.h>
#include <co/y.h>


#include <alg/rng.h>

#define pi (3.141592653589793)
#define FMT CO_REAL_OUT
#define BPART \
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
			r0 = sqrt(rsq);
#define EPART } }
#define BTRI \
	for (i = 0; i < n; i++) { \
		tri3list_get(tri3list, x[i], y[i], z[i]); \
		if (!tri3list_status(tri3list)) continue; \
		vec_get(i, x, y, z, r); \
		t = tri3list_tri(tri3list); \
		gtri(t, /**/ norm); \
		tri3list_point(tri3list, point);
#define ETRI }
enum
{
	X, Y, Z
};
static int n;
#define nx  (25)
#define ny  (25)
#define nz  (25)
static const real c = 10;
static const real mu = 1;
static const real beta = 0.1;
static const real g[3] =
{
	10, 0, 0
};

static const real dt = 0.0005;
static real lo[3] =
{
	-1.2, -1.2, -1.2
};
static real hi[3] =
{
	1.2, 1.2, 1.2
};
PreCons *pre_cons;
PreDensity *pre_density;
PreVisc *pre_visc;
static AlgRng *rng;
static Cell3 *cell;
static Tri3List *tri3list;
static He *he;
static Kernel *kernel;
static real mass, size;
static real *x, *y, *z, *vx, *vy, *vz, *fx, *fy, *fz, *rho, *p, *color, *xm ,*ym, *zm;
static Surface *surface;

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
mesh(real x, real y, real z)
{
	return surface_inside_fast(surface, x, y, z);
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
gtri(int t, /**/ real n[3])
{
	int i, j, k;
	real a[3], b[3], c[3];
	he_tri_ijk(he, t, &i, &j, &k);
	vec_get(i, xm, ym, zm, a);
	vec_get(j, xm, ym, zm, b);
	vec_get(k, xm, ym, zm, c);
	tri_normal(a, b, c, n);
	return CO_OK;
}

static int
force(void)
{
	int i, j, *a;
	real r0, xi, yi, zi, xj, yj, zj, xr, yr, zr, rsq, w, dwr, coeff;
	array_zero3(n, fx, fy, fz);
	array_zero(n, rho);
	cell3_push(cell, n, x, y, z);
	BPART {
		w = kernel_w(kernel, size, r0);
		rho[i] += mass*w;
	}
	EPART

	 for (i = 0; i < n; i++)
		p[i] = eq_state(rho[i]);


	BPART {
		if (j == i) continue;
		dwr = kernel_dwr(kernel, size, r0);
		coeff = p[i]/(rho[i]*rho[i]) + p[j]/(rho[j]*rho[j]);
		coeff *= mass*dwr;
		fx[i]  -= coeff * (xi - xj);
		fy[i]  -= coeff * (yi - yj);
		fz[i] -= coeff * (zi - zj);

		coeff = 1/(rho[i]*rho[j]);
		coeff *= 2*mass*mu*dwr;
		fx[i]  += coeff * (vx[i] - vx[j]);
		fy[i]  += coeff * (vy[i] - vy[j]);
		fz[i] += coeff * (vz[i] - vz[j]);
	}
	EPART

	 return CO_OK;
}


static int
force_bc(void)
{
	int i, j, t, *a;
	real xi, yi, zi, xj, yj, zj, xr, yr, zr, rsq, r0, w, dwr, coeff, fv;
	real point[3], r[3], norm[3], fd[3], dfraction;
	array_zero3(n, fx, fy, fz);
	array_zero(n, rho);
	cell3_push(cell, n, x, y, z);
	body_force(n, x, y, z, fx, fy, fz);
	BPART {
		w = kernel_w(kernel, size, r0);
		rho[i] += mass*w;
	}
	EPART
	    BTRI {
		pre_density_apply(pre_density, r, point, norm, /**/ &dfraction);
		rho[i] += dfraction;
	}
	ETRI

	    for (i = 0; i < n; i++)
		p[i] = eq_state(rho[i]);

	BPART {
		if (j == i) continue;
		dwr = kernel_dwr(kernel, size, r0);
		coeff = p[i]/(rho[i]*rho[i]) + p[j]/(rho[j]*rho[j]);
		coeff *= mass*dwr;
		fx[i]  -= coeff * (xi - xj);
		fy[i]  -= coeff * (yi - yj);
		fz[i] -= coeff * (zi - zj);

		coeff = 1/(rho[i]*rho[j]);
		coeff *= 2*mass*mu*dwr;
		fx[i]  += coeff * (vx[i] - vx[j]);
		fy[i]  += coeff * (vy[i] - vy[j]);
		fz[i] += coeff * (vz[i] - vz[j]);
	}
	EPART

	BTRI {
		pre_cons_apply(pre_cons, r, point, norm, /**/ fd);
		coeff = 2*p[i]/rho[i];
		fx[i] += coeff * fd[X];
		fy[i] += coeff * fd[Y];
		fz[i] += coeff * fd[Z];

		pre_visc_apply(pre_visc, r, point, norm, /**/ &fv);
		coeff = 2*mu/rho[i];
		fx[i]  += coeff*vx[i]*fv;
		fy[i]  += coeff*vy[i]*fv;
		fz[i]  += coeff*vz[i]*fv;
	}
	ETRI
	    return CO_OK;
}

int
dump(int t)
{
	static int First = 1;
	if (t % 10 == 0) {
		if (First) First = 0;
		else printf("\n");
		const real *q[] = {
			x, y, z, vx, vy, vz, rho, color, NULL
		};
		punto_fwrite(n, q, stdout);
		MSG("d " FMT " " FMT, array_min(n, rho), array_max(n, rho));
		MSG("v " FMT " " FMT, array_min(n, vx), array_max(n, vx));
	}
	fflush(stdout);
	return CO_OK;
}

int
main(void)
{
	real V;
	int t, i, j;

	alg_rng_ini(&rng);
	n = nx*ny*nz;
	V = (hi[X] - lo[X])*(hi[Y] - lo[Y])*(hi[Z] - lo[Z]);
	mass = V/n;
	size = 2.5 * (hi[X] - lo[X]) / nx;
	kernel_ini(KERNEL_3D, KERNEL_YANG, &kernel);
	pre_cons_kernel_ini(size, kernel, &pre_cons);
	pre_density_kernel_ini(size, kernel, &pre_density);
	pre_visc_kernel_ini(size, beta, kernel, &pre_visc);
	y_inif(stdin, &he, &xm, &ym, &zm);
	surface_ini(lo, hi, size, &surface);
	surface_update(surface, he, xm, ym, zm);
	tri3list_ini(lo, hi, size, &tri3list);
	tri3list_push(tri3list, he, xm, ym, zm);

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
	MALLOC(n, &color);
	ini(x, y, z);
	cell3_ppp_ini(lo, hi, size, &cell);
	for (t = 0; t < 150; t++) {
		MSG("t %d", t);
		force();
		euler_step(dt,  n, vx, vy, vz, x, y, z);
		cell3_wrap(cell, n, x, y, z);
		euler_step(dt,  n, fx, fy, fz, vx, vy, vz);
		dump(t);
	}
	for (i = j = 0; i < n; i++) {
		if (mesh(x[i], y[i], z[i])) continue;
		x[j] = x[i];
		y[j] = y[i];
		z[j] = z[i];
		j++;
	}
	MSG("nj %d %d", n, j);
	n = j;
	array_zero3(n, vx, vy, vz);
	array_zero3(n, fx, fy, fz);

	for (/**/; t < 80000; t++) {
		MSG("t %d", t);
		force_bc();
		euler_step(dt, n, vx, vy, vz, x, y, z);
		cell3_wrap(cell, n, x, y, z);
		euler_step(dt, n, fx, fy, fz, vx, vy, vz);
		dump(t);
	}

	surface_fin(surface);
	y_fin(he, xm, ym, zm);
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
	FREE(color);
	kernel_fin(kernel);
	pre_cons_fin(pre_cons);
	pre_density_fin(pre_density);
	pre_visc_fin(pre_visc);
	alg_rng_fin(rng);
	tri3list_fin(tri3list);
	MSG("end");
}


/*

pkill morris
CFLAGS=-O3 make
./morris > q
punto -c -G 0:0.3  q

*/
