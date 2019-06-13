#include <stdio.h>
#include <tgmath.h>
#include <omp.h>
#include <real.h>
#include <alg/ode.h>
#include <co/array.h>
#include <co/err.h>
#include <co/force.h>
#include <co/he.h>
#include <co/macro.h>
#include <co/matrix.h>
#include <co/memory.h>
#include <co/segment.h>
#include <co/ode/3.h>
#include <co/off.h>
#include <co/oseen3.h>
#include <co/punto.h>
#include <co/y.h>

#define FMT CO_REAL_OUT

static
Force *Fo[99] =
{
	NULL
};
static He *he;
static Oseen3 *oseen;
static real gdot = 1, mu = 1, la = 0.01, dt = 0.1, tend = 100;
static real *fx, *fy, *fz;
static real *ux, *uy, *uz;
static real *Oxx, *Oxy, *Oxz, *Oyy, *Oyz, *Ozz;
static real *Kxx, *Kxy, *Kxz, *Kyy, *Kyz, *Kzz;
static int n;

static int
fargv(char ***p, He *he)
{
	char *name, **v;
	int i;

	i = 0;
	v = *p;
	while (1) {
		if (v[0] == NULL) break;
		name = v[0];
		v++;
		MSG("%s", name);
		if (!force_good(name)) break;
		force_argv(name, &v, he, &Fo[i]);
		i++;
	}
	*p = v;
	return CO_OK;
}

static int
force(He *he, const real *x, const real *y, const real *z, real *fx, real *fy, real *fz)
{
	int status, i;
	i = 0;
	while (Fo[i]) {
		status = force_force(Fo[i], he, x, y, z, fx, fy, fz);
		if (status != CO_OK)
			ER("force %s failed", force_name(Fo[i]));
		i++;
	}
	return CO_OK;
}


static int
fin(void)
{
	int i;
	i = 0;
	while (Fo[i]) {
		force_fin(Fo[i]);
		i++;
	}
	return CO_OK;
}

#define GET(K) matrix_get(n, n, j, i, (K))
static int
vector_tensor(int n, real s, const real *x, const real *y, const real *z,
	real *Txx, real *Txy, real *Txz, real *Tyy, real *Tyz, real *Tzz,
	real *u, real *v, real *w)
{
	int i;
#pragma omp parallel for
	for (i = 0; i < n; i++) {
		int j;
		real xx, xy, xz, yy, yz, zz, du, dv, dw;
		for (j = 0; j < n; j++) {
			xx = GET(Txx);
			xy = GET(Txy);
			xz = GET(Txz);
			yy = GET(Tyy);
			yz = GET(Tyz);
			zz = GET(Tzz);
			du = xx*x[j] + xy*y[j] + xz*z[j];
			dv = xy*x[j] + yy*y[j] + yz*z[j];
			dw = xz*x[j] + yz*y[j] + zz*z[j];
			u[i] += s*du;
			v[i] += s*dv;
			w[i] += s*dw;
		}
	}
	return CO_OK;
}

static int
F(__UNUSED real t, const real *x, const real *y, const real *z, real *vx,  real *vy, real *vz, __UNUSED void *p0)
{
	int i, k;
	real al, be;

	al = -2/(mu*(1 + la));
	be =  2*(1 - la)/(1 + la) / 40;

	array_zero3(n, fx, fy, fz);
	force(he, x, y, z, fx, fy, fz);
	oseen3_apply(oseen, he, x, y, z, Oxx, Oxy, Oxz, Oyy, Oyz, Ozz);
	oseen3_stresslet(oseen, he, x, y, z, Kxx, Kxy, Kxz, Kyy, Kyz, Kzz);

	array_zero3(n, ux, uy, uz);
	for (i = 0; i < n; i++) ux[i] += gdot*z[i];

	for (k = 0; ; k++) {
		array_zero3(n, vx, vy, vz);
		for (i = 0; i < n; i++) vx[i] += gdot*z[i];
		vector_tensor(n, al, fx, fy, fz, Oxx, Oxy, Oxz, Oyy, Oyz, Ozz, vx, vy, vz);
		vector_tensor(n, be, ux, uy, uz, Kxx, Kxy, Kxz, Kyy, Kyz, Kzz, vx, vy, vz);
		//MSG("%03d " FMT " " FMT " " FMT, k, array_l2(n, vx, ux), array_l2(n, vy, uy), array_l2(n, vz, uz));
		if (k == 100)
			break;
		array_copy3(n, vx, vy, vz, ux, uy, uz);
	}
	return CO_OK;
}

static int
tensor_ini(int n, real **xx, real **xy, real **xz, real **yy, real **yz, real **zz)
{
	matrix_ini(n, n, xx);
	matrix_ini(n, n, xy);
	matrix_ini(n, n, xz);
	matrix_ini(n, n, yy);
	matrix_ini(n, n, yz);
	matrix_ini(n, n, zz);
	return CO_OK;
}

static int
tensor_fin(real *xx, real *xy, real *xz, real *yy, real *yz, real *zz)
{
	matrix_fin(xx);
	matrix_fin(xy);
	matrix_fin(xz);
	matrix_fin(yy);
	matrix_fin(yz);
	matrix_fin(zz);
	return CO_OK;
}

int
main(__UNUSED int argc, char **argv)
{
	real *x, *y, *z;
	real e;
	int k;
	real t, time;
	char file[999];
	Ode3 *ode;

	err_set_ignore();
	argv++;
	y_inif(stdin, &he, &x, &y, &z);
	fargv(&argv, he);
	n = he_nv(he);
	e = segment_average(he, x, y, y)/5;
	MSG("e " FMT, e);
	oseen3_ini(he, e, &oseen);
	ode3_ini(RKF45, n, dt/10, F, NULL, &ode);
	MALLOC3(n, &fx, &fy, &fz);
	MALLOC3(n, &ux, &uy, &uz);
	tensor_ini(n, &Oxx, &Oxy, &Oxz, &Oyy, &Oyz, &Ozz);
	tensor_ini(n, &Kxx, &Kxy, &Kxz, &Kyy, &Kyz, &Kzz);
	k = 0;
	t = time = 0;
	while (time < tend) {
		t = time + dt;
		ode3_apply(ode, &time, t, x, y, z);
		sprintf(file, "%05d.off", k++);
		off_he_xyz_write(he, x, y, z, file);
		MSG("%s", file);
	}
	FREE3(fx, fy, fz);
	FREE3(ux, uy, uz);
	tensor_fin(Oxx, Oxy, Oxz, Oyy, Oyz, Ozz);
	tensor_fin(Kxx, Kxy, Kxz, Kyy, Kyz, Kzz);
	oseen3_fin(oseen);
	ode3_fin(ode);
	fin();
	y_fin(he, x, y, z);
}

/*

Put

git clean -fdxq
m
f=/u/.co/sph/icosa/Nt320.off A=12.3299 V=4.0470
#f=/u/.co/sph/icosa/Nt80.off A=11.6659 V=3.65871
#f=/u/.co/sph/icosa/Nt20.off A=9.57454 V=2.53615
#f=/u/.co/rbc/laplace/0.off A=8.66899 V=1.53405
./3l garea $A 1e2 volume $V 1e2 strain $f lim 1 1 0 0 0 0  juelicher_xin 0.1 0 0 0 < $f

co.geomview  -t -0.0208784 0.0709866 4.07545e-09 -r 55.8221 -0.28266 0.693395 -f 28 *.off

Kill git

*/
