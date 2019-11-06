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

int
main(__UNUSED int argc, char **argv)
{
	Skel *skel;
	real *x, *y, *vx, *vy, *fx, *fy;
	char file[9999];
	FILE *f;
	int n, i, j, nstep = 100000;
	real dt = 1e-3;

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

	for (i = j = 0; i < nstep; i++) {
		array_zero(n, fx);
		array_zero(n, fy);
		force(skel, x, y, fx, fy);
		//euler_step(dt, n, vx, vy, x, y);
		//euler_step(-dt, n, fx, fy, vx, vy);
		euler_step(-dt, n, fx, fy, x, y);

		if (i % 1000 == 0) {
			sprintf(file, "%05d.skel", j++);
			f = fopen(file, "w");
			skel_write(skel, x, y, f);
			fclose(f);
		}
	}
	FREE(vx);
	FREE(vy);
	FREE(fx);
	FREE(fy);
	fin();
	skel_xy_fin(x, y, skel);
}

/*

git clean -fdxq
m
A=0.8835572001943658
./relax len data/100.skel 1 0 0    area $A 1     bend_min 1e-3 < data/100.skel; echo end
co.geomview -a /u/a -p cat *.skel

*/