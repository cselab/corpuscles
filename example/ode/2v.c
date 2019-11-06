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
#include <co/ode/2v.h>
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
F(__UNUSED real t, const real *x, const real *y, __UNUSED const real *vx, __UNUSED const real *vy, real *fx,  real *fy, void *p0)
{
	Param *p;
	Skel *skel;
	p = p0;
	skel = p->skel;
	force(skel, x, y, fx, fy);
	return CO_OK;
}

int
main(__UNUSED int argc, char **argv)
{
	Skel *skel;
	real *x, *y, *vx, *vy;
	char file[9999];
	FILE *s;
	int j, n;
	real dt = 1, tend = 100;
	Ode2v *ode;
	Param p;

	argv++;
	skel_read(stdin, &x, &y, &skel);
	fargv(&argv, skel);
	n = skel_nv(skel);
	MSG("area " FMT, area2(skel, x, y));
	MSG("len " FMT, len(skel, x, y));
	p.skel = skel;
	ode2v_ini(RKCK, n, dt, F, &p, &ode);
	CALLOC(n, &vx);
	CALLOC(n, &vy);

	j = 0;
	real t, time = 0;
	while (time < tend) {
		t = time + dt;
		ode2v_apply(ode, &time, t, x, y, vx, vy);
		sprintf(file, "%05d.skel", j++);
		s = fopen(file, "w");
		skel_write(skel, x, y, s);
		MSG(FMT " " FMT, vy[0], y[0]);
		fclose(s);
	}

	ode2v_fin(ode);
	fin();
	FREE(vx);
	FREE(vy);
	skel_xy_fin(x, y, skel);
}

/*

Put
git clean -fdxq
m lint
A=0.65
L=4.926052821288913
./2v glen $L 1    len data/100.skel 1 0.1 0    area $A 1     bend_min 5e-3 < data/100.skel; echo end
sh /u/bin/co.geomview -a /u/a -p cat *.skel
Kill git
win
*/
