#include <stdio.h>
#include <math.h>
#include <real.h>
#include <co/area2.h>
#include <co/array.h>
#include <co/err.h>
#include <co/len.h>
#include <co/macro.h>
#include <co/memory.h>
#include <co/skel.h>
#include <co/force2.h>

#include <alg/ode.h>

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
	int n;
	real *x, *y, *fx, *fy;
	Skel *skel;
};

static int
F(__UNUSED real t, const real *r, real *f, void *p0)
{
	Param *p;
	int i, n;
	real *x, *y, *fx, *fy;
	Skel *skel;

	p = p0;
	n = p->n;
	x = p->x;
	y = p->y;
	fx = p->fx;
	fy = p->fy;
	skel = p->skel;

	for (i = 0; i < n; i++) {
		x[i] = r[i];
		y[i] = r[n + i];
	}
	array_zero(n, fx);
	array_zero(n, fy);
	force(skel, x, y, fx, fy);
	for (i = 0; i < n; i++) {
		f[i] = -fx[i];
		f[i + n] = -fy[i];
	}

	return CO_OK;
}

int
main(__UNUSED int argc, char **argv)
{
	Skel *skel;
	real *x, *y, *vx, *vy, *fx, *fy, *r, *f;
	char file[9999];
	FILE *s;
	int n, i, j;
	real dt = 1, tend = 1000;
	Ode *ode;
	Param p;

	argv++;
	skel_read(stdin, &x, &y, &skel);
	fargv(&argv, skel);
	n = skel_nv(skel);
	MSG("area " FMT, area2(skel, x, y));
	MSG("len " FMT, len(skel, x, y));

	CALLOC(n, &vx);
	CALLOC(n, &vy);
	CALLOC(n, &fx);
	CALLOC(n, &fy);
	MALLOC(2*n, &r);
	MALLOC(2*n, &f);
	p.skel = skel;
	p.x = x;
	p.y = y;
	p.fx = fx;
	p.fy = fy;
	p.n = n;
	ode_ini(RKF45, 2*n, dt, F, &p, &ode);

	for (i = 0; i < n; i++) {
		r[i] = x[i];
		r[n + i] = y[i];
	}

	j = 0;
	real t, time = 0;
	while (time < tend) {
		t = time + dt;
		ode_apply(ode, &time, t, r);
		sprintf(file, "%05d.skel", j++);
		s = fopen(file, "w");
		skel_write(skel, x, y, s);
		MSG(FMT " " FMT, time, y[0]);
		fclose(s);
	}

	ode_fin(ode);
	FREE(vx);
	FREE(vy);
	FREE(fx);
	FREE(fy);
	FREE(f);
	FREE(r);
	fin();
	skel_xy_fin(x, y, skel);
}

/*

Put
git clean -fdxq
m lint
A=0.8835572001943658
./main len data/100.skel 1 0 0    area $A 1     bend_min 1e-3 < data/100.skel; echo end
co.geomview -a /u/a -p cat *.skel
Kill git
*/
