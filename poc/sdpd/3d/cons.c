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
#include <co/pre/density.h>
#include <co/pre/cons.h>
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
		gtri(t, /**/ point, norm);
#define ETRI }

enum
{
	X, Y, Z
};

static int n;
#define nx  (40)
#define ny  (40)
#define nz  (20)
static const real c = 10;
static real lo[3] = 
{
	-1.2, -1.2, -0.6
};
static real hi[3] = 
{
	1.2, 1.2, 0.6
};
PreDensity *pre_density;
PreCons *pre_cons;
static AlgRng *rng;
static Cell3 *cell;
static Tri3List *tri3list;
static He *he;
static Kernel *kernel;
static real mass, size;
static real *x, *y, *z, *rho, *p, *fx, *fy, *fz, *xm ,*ym, *zm;
static Surface *surface;

static real
eq_state(real rho)
{
	return c*c*rho;
}

static int
grid(real *x, real *y, real *z)
{
	real x0, y0, z0, dx,a, b;
	int i, j, k, m;
	dx = (hi[X] - lo[X])/nx;
	a = -0.0*dx;
	b =   0.0*dx;
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

static int
mesh(real x, real y, real z)
{
	return surface_inside_fast(surface, x, y, z);
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
	}
	EPART

	    return CO_OK;
}

static int
force_bc(void)
{
	int i, j, t, *a;
	real xi, yi, zi, xj, yj, zj, xr, yr, zr, rsq, r0, w, dwr, coeff;
	real point[3], r[3], norm[3], fd[3], dfraction;
	array_zero3(n, fx, fy, fz);
	array_zero(n, rho);
	cell3_push(cell, n, x, y, z);
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
		fx[i]  += coeff * (xi - xj);
		fy[i]  += coeff * (yi - yj);
		fz[i] += coeff * (zi - zj);
	}
	EPART

	    BTRI {
		pre_cons_apply(pre_cons, r, point, norm, /**/ fd);
		coeff = -2*p[i]/rho[i];
		fx[i] += coeff * fd[X];
		fy[i] += coeff * fd[Y];
		fz[i] += coeff * fd[Z];
	}
	ETRI

	    return CO_OK;
}

int
dump(void)
{
	static int First = 1;
	if (First) First = 0;
	else printf("\n");
	const real *q[] = {
		x, y, z, rho, fx, fy, fz, NULL
	};
	punto_fwrite(n, q, stdout);
	MSG("rho: " FMT " " FMT " " FMT, array_min(n, rho), array_mean(n, rho), array_max(n, rho));
	MSG("fx: " FMT " " FMT " " FMT, array_min(n, fx), array_mean(n, fx), array_max(n, fx));
	MSG("fz: " FMT " " FMT " " FMT, array_min(n, fz), array_mean(n, fz), array_max(n, fz));
	MSG("=");
	fflush(stdout);
	return CO_OK;
}

int
main(void)
{
	real V;
	int i, j;

	alg_rng_ini(&rng);
	n = nx*ny*nz;
	V = (hi[X] - lo[X])*(hi[Y] - lo[Y])*(hi[Z] - lo[Z]);
	mass = V/n;
	size = 2.5 * (hi[X] - lo[X]) / nx;
	kernel_ini(KERNEL_3D, KERNEL_YANG, &kernel);
	pre_density_kernel_ini(size, kernel, &pre_density);
	pre_cons_kernel_ini(size, kernel, &pre_cons);
	y_inif(stdin, &he, &xm, &ym, &zm);

	surface_ini(lo, hi, size/4, &surface);
	surface_update(surface, he, xm, ym, zm);

	tri3list_ini(lo, hi, size, &tri3list);
	tri3list_push(tri3list, he, xm, ym, zm);

	MALLOC(n, &x);
	MALLOC(n, &y);
	MALLOC(n, &z);
	MALLOC(n, &fx);
	MALLOC(n, &fy);
	MALLOC(n, &fz);
	MALLOC(n, &rho);
	MALLOC(n, &p);
	ini(x, y, z);
	cell3_ppp_ini(lo, hi, size, &cell);
	force();
	dump();

	for (i = j = 0; i < n; i++) {
		if (mesh(x[i], y[i], z[i])) continue;
		x[j] = x[i];
		y[j] = y[i];
		z[j] = z[i];
		j++;
	}
	n = j;
	force_bc();
	dump();
	surface_fin(surface);
	y_fin(he, xm, ym, zm);
	cell3_fin(cell);
	FREE(x);
	FREE(y);
	FREE(z);
	FREE(fx);
	FREE(fy);
	FREE(fz);
	FREE(p);
	FREE(rho);
	kernel_fin(kernel);
	pre_density_fin(pre_density);
	pre_cons_fin(pre_cons);
	alg_rng_fin(rng);
	tri3list_fin(tri3list);
	MSG("end");
}
