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
		gtri(t, /**/ point, norm);
#define ETRI }

enum
{
	X, Y, Z
};

static int n;
#define nx  (40)
#define ny  (40)
#define nz  (40)
static const real mu = 1;
static const real beta = 0.1;
static real lo[3] = 
{
	-1.2, -1.2, -1.2
};
static real hi[3] = 
{
	1.2, 1.2, 1.2
};
PreDensity *pre_density;
PreVisc *pre_visc;
static AlgRng *rng;
static Cell3 *cell;
static Tri3List *tri3list;
static He *he;
static Kernel *kernel;
static real mass, size;
static real *x, *y, *z, *rho, *fx, *fy, *fz, *vx, *vy, *vz, *xm ,*ym, *zm;
static Surface *surface;

static int
grid(real *x, real *y, real *z)
{
	real x0, y0, z0, dx,a, b;
	int i, j, k, m;
	dx = (hi[X] - lo[X])/nx;
	a = -0.1*dx;
	b =   0.1*dx;
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
ini_v0(real x, real y, real z, real *u, real *v, real *w)
{
	real r, r2, r3, dx, dy, dz;
	r = sqrt(x*x + y*y + z*z);
	if (r < 0.5) {
		*u = *v = *w = 0;
		return CO_OK;
	}
	r = 1/r;
	r2 = r*r;
	r3 = r*r*r;
	dx = x*x*r2;
	dy = x*y*r2;
	dz = x*z*r2;
	*u = -3.0/4*r*(1 + dx) - 1.0/4*r3*(1 - 3*dx) + 1;
	*v = -3.0/4*r*dy          - 1.0/4*r3*(-3*dy);
	*w = -3.0/4*r*dz          - 1.0/4*r3*(-3*dz);
	return CO_OK;		
}

static int
ini_v(const real *x, const real *y, const real *z, real *vx, real *vy, real *vz)
{
	int i;
	for (i = 0; i < n; i++)
		ini_v0(x[i], y[i], z[i], &vx[i], &vy[i], &vz[i]);
	return CO_OK;
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

	BPART {
		if (j == i) continue;
		dwr = kernel_dwr(kernel, size, r0);
		coeff = 1/(rho[i]*rho[j]);
		coeff *= 2*mass*mu*dwr;
		fx[i]  += coeff*(vx[i] - vx[j]);
		fy[i]  += coeff*(vy[i] - vy[j]);
		fz[i] += coeff*(vz[i] - vz[j]);
	}
	EPART

	    return CO_OK;
}

static int
force_bc(void)
{
	int i, j, t, *a;
	real xi, yi, zi, xj, yj, zj, xr, yr, zr, rsq, r0, w, dwr, coeff, fd, nd;
	real point[3], r[3], norm[3], dfraction;
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

	BPART {
		if (j == i) continue;
		dwr = kernel_dwr(kernel, size, r0);
		coeff = 1/(rho[i]*rho[j]);
		coeff *= 2*mass*mu*dwr;
		fx[i]  += coeff * (vx[i] - vx[j]);
		fy[i]  += coeff * (vy[i] - vy[j]);
		fz[i] += coeff * (vz[i] - vz[j]);
	}
	EPART

	BTRI {
		pre_visc_apply(pre_visc, r, point, norm, /**/ &fd);
		nd = rho[i]/mass;
		coeff =  2*mass*mu/(rho[i]*rho[i])*nd;
		fx[i] += coeff * vx[i]*fd;
		fy[i] += coeff * vy[i]*fd;
		fz[i] += coeff * vz[i]*fd;
	}
	ETRI
	return CO_OK;
}

int
dump(void)
{
	int i;
	static int First = 1;
	for (i = 0; i < n; i++)
		if (mesh(x[i], y[i], z[i])) fx[i] = fy[i] = fz[i] = 0;	
	if (First) First = 0;
	else printf("\n");
	const real *q[] = {
		x, y, z, rho, fx, fy, fz, vx, vy, vz, NULL
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
	pre_visc_kernel_ini(size, beta, kernel, &pre_visc);
	y_inif(stdin, &he, &xm, &ym, &zm);

	surface_ini(lo, hi, size/2, &surface);
	surface_update(surface, he, xm, ym, zm);

	tri3list_ini(lo, hi, size, &tri3list);
	tri3list_push(tri3list, he, xm, ym, zm);

	MALLOC(n, &x);
	MALLOC(n, &y);
	MALLOC(n, &z);
	MALLOC(n, &vx);
	MALLOC(n, &vy);
	MALLOC(n, &vz);
	MALLOC(n, &fx);
	MALLOC(n, &fy);
	MALLOC(n, &fz);
	MALLOC(n, &rho);
	ini(x, y, z);
	ini_v(x, y, z, vx, vy, vz);
	cell3_ppp_ini(lo, hi, size, &cell);
	force();
	dump();
	for (i = j = 0; i < n; i++) {
		if (mesh(x[i], y[i], z[i])) continue;
		x[j] = x[i];
		y[j] = y[i];
		z[j] = z[i];
		vx[j] = vx[i];
		vy[j] = vy[i];
		vz[j] = vz[i];
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
	FREE(vx);
	FREE(vy);
	FREE(vz);
	FREE(rho);
	kernel_fin(kernel);
	pre_density_fin(pre_density);
	pre_visc_fin(pre_visc);
	alg_rng_fin(rng);
	tri3list_fin(tri3list);
	MSG("end");
}
