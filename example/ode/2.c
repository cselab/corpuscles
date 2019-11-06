#include <stdio.h>
#include <math.h>
#include <real.h>
#include <alg/ode.h>
#include <co/area2.h>
#include <co/array.h>
#include <co/err.h>
#include <co/force2.h>
#include <co/len.h>
#include <co/macro.h>
#include <co/memory.h>
#include <co/ode/2.h>
#include <co/skel.h>

#define FMT CO_REAL_OUT

static
Force2 *Force[99] =
{
	NULL
};

static int
fargv(char ***p, Skel *skel)
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
		if (!force2_good(name)) break;
		force2_argv(name, &v, skel, &Force[i]);
		i++;
	}

	*p = v;
	return CO_OK;
}

static int
force(Skel *skel, const real *x, const real *y, real *fx, real *fy)
{
	int i;
	i = 0;
	while (Force[i]) {
		force2_force(Force[i], skel, x, y, fx, fy);
		i++;
	}
	return CO_OK;
}


static int
fin(void)
{
	int i;
	i = 0;
	while (Force[i]) {
		force2_fin(Force[i]);
		i++;
	}
	return CO_OK;
}


typedef struct Param Param;
struct Param
{
	Skel *skel;
};


static int
F(__UNUSED real t, const real *x, const real *y, real *fx,  real *fy, void *p0)
{
	Param *p;
	Skel *skel;
	int n;

	p = p0;
	skel = p->skel;
	n = skel_nv(skel);
	force(skel, x, y, fx, fy);
	array_neg(n, fx);
	array_neg(n, fy);
	return CO_OK;
}

int
main(__UNUSED int argc, char **argv)
{
	Skel *skel;
	real *x, *y;
	char file[9999];
	FILE *s;
	int j, n;
	real dt = 1, tend = 1000;
	Ode2 *ode;
	Param p;

	argv++;
	skel_read(stdin, &x, &y, &skel);
	fargv(&argv, skel);
	n = skel_nv(skel);
	MSG("area " FMT, area2(skel, x, y));
	MSG("len " FMT, len(skel, x, y));
	p.skel = skel;
	ode2_ini(RKCK, n, dt, F, &p, &ode);

	j = 0;
	real t, time = 0;
	while (time < tend) {
		t = time + dt;
		ode2_apply(ode, &time, t, x, y);
		sprintf(file, "%05d.skel", j++);
		s = fopen(file, "w");
		skel_write(skel, x, y, s);
		MSG(FMT " " FMT, time, y[0]);
		fclose(s);
	}

	ode2_fin(ode);
	fin();
	skel_xy_fin(x, y, skel);
}

/*

Put
git clean -fdxq
m lint
A=0.8
L=4.926052821288913
./2 glen $L 1    len data/circle.skel 1 0.1 0    area $A 1     bend_min 5e-3 < data/circle.skel; echo end
co.geomview -a /u/a -p cat *00.skel
Kill git
cd /u/alg ; push hub

*/
