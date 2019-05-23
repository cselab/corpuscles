#include <stdio.h>
#include <tgmath.h>
#include <real.h>
#include <alg/ode.h>
#include <co/array.h>
#include <co/len.h>
#include <co/err.h>
#include <co/force.h>
#include <co/macro.h>
#include <co/matrix.h>
#include <co/memory.h>
#include <co/ode/3.h>
#include <co/oseen3.h>
#include <co/he.h>
#include <co/punto.h>
#include <co/y.h>
#include <co/off.h>

#define FMT CO_REAL_OUT

static
Force *Fo[99] =
{
	NULL
};
static He *he;
static Oseen3 *oseen;
static real gamma = 1, mu = 1, dt = 1e-2, tend = 100;
static real *fx, *fy, *fz;
static real *Oxx, *Oxy, *Oxz, *Oyy, *Oyz, *Ozz;
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
	int i;
	i = 0;
	while (Fo[i]) {
		force_force(Fo[i], he, x, y, z, fx, fy, fz);
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

static int
F(__UNUSED real t, const real *x, const real *y, const real *z, real *vx,  real *vy, real *vz, __UNUSED void *p0)
{
	int i, ga, be;
	real xx, xy, xz, yy, yz, zz;
	for (i = 0; i < n; i++) {
		vx[i] = gamma*z[i];
		vy[i] = vz[i] = 0;
	}
	array_zero3(n, fx, fy, fz);
	force(he, x, y, z, fx, fy, fz);
	oseen3_apply(oseen, he, x, y, z, Oxx, Oxy, Oxz, Oyy, Oyz, Ozz);
	for (ga = 0; ga < n; ga++)
		for (be = 0; be < n; be++) {
			xx = matrix_get(n, n, be, ga, Oxx)/mu;
			xy = matrix_get(n, n, be, ga, Oxy)/mu;
			xz = matrix_get(n, n, be, ga, Oxz)/mu;
			yy = matrix_get(n, n, be, ga, Oyy)/mu;
			yz = matrix_get(n, n, be, ga, Oyz)/mu;
			zz = matrix_get(n, n, be, ga, Ozz)/mu;
			vx[be] -= xx*fx[ga] + xy*fy[ga] + xz*fz[ga];
			vy[be] -= xy*fx[ga] + yy*fy[ga] + yz*fz[ga];
			vz[be] -=  xz*fx[ga] + yz*fy[ga] + zz*fz[ga];
		}
	for (i = 0; i < n; i++) {
		vx[i] = -vx[i];
		vy[i] = -vy[i];
		vz[i] = -vz[i];
	}
	//MSG("t " FMT, t);
	return CO_OK;
}

int
main(__UNUSED int argc, char **argv)
{
	real *x, *y, *z, *vx, *vy, *vz;
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
	e = 0.025;
	oseen3_ini(e, &oseen);
	ode3_ini(RK4, n, dt/10, F, NULL, &ode);

	CALLOC3(n, &vx, &vy, &vz);
	CALLOC3(n, &fx, &fy, &fz);
	matrix_ini(n, n, &Oxx);
	matrix_ini(n, n, &Oxy);
	matrix_ini(n, n, &Oxz);
	matrix_ini(n, n, &Oyy);
	matrix_ini(n, n, &Oyz);
	matrix_ini(n, n, &Ozz);
	k = 0;
	t = time = 0;
	while (time < tend) {
		t = time + dt;
		ode3_apply(ode, &time, t, x, y, z);
		MSG("x[0] " FMT, x[0]);
		sprintf(file, "%05d.off", k++);
		off_he_xyz_write(he, x, y, z, file);
	}
	FREE3(vx, vy, vz);
	FREE3(fx, fy, fz);
	matrix_fin(Oxx);
	matrix_fin(Oxy);
	matrix_fin(Oxz);
	matrix_fin(Oyy);
	matrix_fin(Oyz);
	matrix_fin(Ozz);
	oseen3_fin(oseen);
	ode3_fin(ode);
	fin();
	y_fin(he, x, y, z);
}

/*

Put

git clean -fdxq
m lint
#f=/u/.co/sph/icosa/Nt20.off
#A=9.57454 V=2.53615
f=/u/.co/rbc/laplace/1.off
A=8.66899 V=1.53405
./3d garea $A 10 volume $V 10 strain $f lim 0.01 1 0 0 0 0  juelicher_xin 0.1 0 0 0 < $f

co.geomview  -t -0.0208784 0.0709866 4.07545e-09 -r 55.8221 -0.28266 0.693395 -f 23.5861 *.off


Kill git

*/
