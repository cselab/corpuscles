#include <stdio.h>
#include <math.h>
#include <real.h>
#include <alg/ode.h>
#include <co/array.h>
#include <co/err.h>
#include <co/force.h>
#include <co/he.h>
#include <co/len.h>
#include <co/macro.h>
#include <co/memory.h>
#include <co/ode/3.h>
#include <co/off.h>
#include <co/y.h>

#define FMT CO_REAL_OUT

static
Force *Fo[99] =
{
	NULL
};

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


typedef struct Param Param;
struct Param
{
	He *he;
};


static int
F(__UNUSED real t, const real *x, const real *y, const real *z, real *fx,  real *fy, real *fz, void *p0)
{
	Param *p;
	He *he;
	p = p0;
	he = p->he;
	force(he, x, y, z, fx, fy, fz);
	return CO_OK;
}

int
main(__UNUSED int argc, char **argv)
{
	He *he;
	real *x, *y, *z;
	char file[9999];
	int j, n;
	real dt = 1, tend = 100;
	Ode3 *ode;
	Param p;

	argv++;
	y_inif(stdin, &he, &x, &y, &z);
	fargv(&argv, he);
	n = he_nv(he);
	p.he = he;
	ode3_ini(RKCK, n, dt/100, F, &p, &ode);

	j = 0;
	real t, time = 0;
	while (time < tend) {
		t = time + dt;
		ode3_apply(ode, &time, t, x, y, z);
		sprintf(file, "%05d.off", j++);
		off_he_xyz_write(he, x, y, z, file);
		MSG(FMT " " FMT, time, x[0]);
	}
	ode3_fin(ode);
	fin();
	y_fin(he, x, y, z);
}

/*

Put

git clean -fdxq
m lint
f=/u/.co/rbc/laplace/0.off
A=8.66899 # V=1.53405
V=6.0
./3 garea $A 10 volume $V 10 juelicher_xin 0.1 0 0 0 < $f
co.geomview -r 90 0 0 -f 38 *.off
Kill git

*/
