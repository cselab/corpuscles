#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <real.h>

#include <co/array.h>
#include <co/cell2.h>
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
		xi = x[i]; yi = y[i]; \
		cell2_parts(cell, xi, yi, &a); \
		while ( (j = *a++) != -1) { \
			xj = x[j]; yj = y[j];  \
			cell2_bring(cell, xi, yi, &xj, &yj); \
			xr = xi - xj; \
			yr = yi  - yj; \
			rsq = xr*xr + yr*yr; \
			if (rsq > size*size) continue; \
			r0 = sqrt(rsq);
#define EPART } }
#define BTRI \
	for (i = 0; i < n; i++) { \
		tri3list_get(tri3list, x[i], y[i], z[i]); \
		if (!tri3list_status(tri3list)) continue; \
		vec_get(i, x, y, z, r); \
		t = tri3list_tri(tri3list); \
		gtri(t, /**/ point, norm);
#define ETRI }
enum
{
	X, Y
};
static int n;
#define nx  (42)
#define ny  (42)
static const real c = 10;
static const real beta = 0.1;
static const real mu = 1;
static const real g[2] =
{
	10, 0
};

static const real dt = 1e-4;
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
static Cell2 *cell;
static Tri3List *tri3list;
static He *he;
static Kernel *kernel;
static real mass, size;
static real *x, *y, *z, *vx, *vy, *fx, *fy, *rho, *p, *xm ,*ym, *zm;
static Surface *surface;

static int
rnd(real *x, real *y, real *z)
{
	int i;
	for (i = 0; i < n; i++) {
		x[i] = alg_rng_uniform(rng, lo[X], hi[X]);
		y[i] = alg_rng_uniform(rng, lo[Y], hi[Y]);
		z[i]  = 0;
	}
	return CO_OK;
}

static int
ini(real *x, real *y, real *z)
{
	return rnd(x, y, z);
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
mesh(real x, real y, real z)
{
	return surface_inside_fast(surface, x, y, z);
}

static int
body_force(int n,  __UNUSED const real *x, __UNUSED const real *y, real *fx,  real *fy)
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

static int
gtri(int t, /**/ real p[3], real n[3])
{
	int i, j, k;
	real a[3], b[3], c[3];
	he_tri_ijk(he, t, &i, &j, &k);
	vec_get(i, xm, ym, zm, a);
	vec_get(j, xm, ym, zm, b);
	vec_get(k, xm, ym, zm, c);
	tri_center(a, b, c, p);
	tri_normal(a, b, c, n);
	return CO_OK;
}

static int
force(void)
{
	int i, j, *a;
	real r0, xi, yi, xj, yj, xr, yr, rsq, w, dwr, coeff;
	array_zero(n, fx);
	array_zero(n, fy);
	array_zero(n, rho);
	cell2_push(cell, n, x, y);
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

		coeff = 1/(rho[i]*rho[j]);
		coeff *= 2*mass*mu*dwr;
		fx[i]  += coeff * (vx[i] - vx[j]);
		fy[i]  += coeff * (vy[i] - vy[j]);
	}
	EPART

	 return CO_OK;
}


static int
force_bc(void)
{
	int i, j, t, *a;
	real xi, yi, xj, yj, xr, yr, rsq, r0, w, dwr, coeff;
	real point[3], r[3], norm[3], fd[3], fv, dfraction;
	array_zero(n, fx);
	array_zero(n, fy);
	array_zero(n, rho);
	cell2_push(cell, n, x, y);
	body_force(n, x, y, fx, fy);
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

		coeff = 1/(rho[i]*rho[j]);
		coeff *= 2*mass*mu*dwr;
		fx[i]  += coeff*(vx[i] - vx[j]);
		fy[i]  += coeff*(vy[i] - vy[j]);
	}
	EPART

	BTRI {
		pre_cons_apply(pre_cons, r, point, norm, /**/ fd);
		coeff =  2*p[i]/rho[i];
		fx[i] += coeff*fd[X];
		fy[i] += coeff*fd[Y];

		pre_visc_apply(pre_visc, r, point, norm, /**/ &fv);
		coeff = 2*mu/rho[i];
		fx[i]  += coeff*vx[i]*fv;
		fy[i]  += coeff*vy[i]*fv;
	}
	ETRI
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
			x, y, vx, vy, rho, fx, fy, NULL
		};
		punto_fwrite(n, q, stdout);		
		fprintf(stderr, 
			"%06d d " FMT " " FMT "\n",
			t, array_min(n, rho), array_max(n, rho));
		fprintf(stderr, 
			"%06d v " FMT " " FMT "\n",
			t, array_min(n, vx), array_max(n, vx));
	}
	fflush(stdout);
	return CO_OK;
}

int
main(void)
{
	real V;
	int t, i, j;

	err_set_ignore();
	alg_rng_ini(&rng);
	n = nx*ny;
	V = (hi[X] - lo[X])*(hi[Y] - lo[Y]);
	mass = V/n;
	size = 2.5*(hi[X] - lo[X])/nx;
	kernel_ini(KERNEL_2D, KERNEL_YANG, &kernel);
	pre_cons2_kernel_ini(size, kernel, &pre_cons);
	pre_density2_kernel_ini(size, kernel, &pre_density);
	pre_visc2_kernel_ini(size, beta, kernel, &pre_visc);
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
	MALLOC(n, &fx);
	MALLOC(n, &fy);
	MALLOC(n, &rho);
	MALLOC(n, &p);
	ini(x, y, z);
	cell2_pp_ini(lo, hi, size, &cell);
	for (t = 0; t < 1000; t++) {
		force();
		euler_step(dt,  n, vx, vy, x, y);
		cell2_wrap(cell, n, x, y);
		euler_step(dt,  n, fx, fy, vx, vy);
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
	array_zero(n, vx);
	array_zero(n, vy);
	array_zero(n, fx);
	array_zero(n, fy);

	for (/**/; t < 80000; t++) {
		force_bc();
		euler_step(dt, n, vx, vy, x, y);
		cell2_wrap(cell, n, x, y);
		euler_step(dt, n, fx, fy, vx, vy);
		dump(t);
	}

	surface_fin(surface);
	y_fin(he, xm, ym, zm);
	cell2_fin(cell);
	FREE(x);
	FREE(y);
	FREE(z);
	FREE(vx);
	FREE(vy);
	FREE(fx);
	FREE(fy);
	FREE(rho);
	FREE(p);
	kernel_fin(kernel);
	pre_cons_fin(pre_cons);
	pre_density_fin(pre_density);
	pre_visc_fin(pre_visc);
	alg_rng_fin(rng);
	tri3list_fin(tri3list);
	MSG("end");
}
