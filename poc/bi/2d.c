#include <stdio.h>
#include <tgmath.h>
#include <real.h>
#include <co/array.h>
#include <co/len.h>
#include <co/err.h>
#include <co/force2.h>
#include <co/macro.h>
#include <co/matrix.h>
#include <co/memory.h>
#include <co/oseen2.h>
#include <co/skel.h>
#include <co/punto.h>

#define FMT CO_REAL_OUT

static
Force2 *Force[99] =
{
	NULL
};
static Skel *skel;
static real gamma = 0.1, mu = 0.1, dt = 2e-6;

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

int
main(__UNUSED int argc, char **argv)
{
	real *x, *y, *vx, *vy, *fx, *fy;
	real *Oxx, *Oxy, *Oyy;
	real xx, xy, yy;
	int n, i, j;
	int be, ga;

	argv++;
	skel_read(stdin, &x, &y, &skel);
	fargv(&argv, skel);
	n = skel_nv(skel);

	MSG("len " FMT, len(skel, x, y));
	CALLOC2(n, &vx, &vy);
	CALLOC2(n, &fx, &fy);
	matrix_ini(n, n, &Oxx);
	matrix_ini(n, n, &Oxy);
	matrix_ini(n, n, &Oyy);

	for (j = 0; j < 1000000; j++) {
		for (i = 0; i < n; i++) {
			vx[i] = gamma*y[i];
			vy[i] = 0;
		}
		array_zero(n, fx);
		array_zero(n, fy);
		force(skel, x, y, fx, fy);
		oseen2(skel, x, y, Oxx, Oxy, Oyy);
		for (ga = 0; ga < n; ga++)
			for (be = 0; be < n; be++) {
				xx = matrix_get(n, n, be, ga, Oxx)/mu;
				xy = matrix_get(n, n, be, ga, Oxy)/mu;
				yy = matrix_get(n, n, be, ga, Oyy)/mu;
				vx[be] += xx*fx[ga] + xy*fy[ga];
				vy[be] += xy*fx[ga] + yy*fy[ga];
			}

		FILE *f;
		char file[9999];
		for (i = 0; i < n; i++) {
			x[i] += dt*vx[i];
			y[i] += dt*vy[i];
		}
		if (j % 1000 == 0) {
			MSG("x[0] " FMT, x[0]);
			sprintf(file, "%05d.skel", j);
			f = fopen(file, "w");
			skel_write(skel, x, y, f);
			fclose(f);
			const real *q[] = 
			{
				x, y, vx, vy, fx, fy, NULL
			};
			punto_fwrite(n, q, stdout);
			printf("\n");
		}
	}

	FREE2(vx, vy);
	FREE2(fx, fy);
	matrix_fin(Oxx);
	matrix_fin(Oxy);
	matrix_fin(Oyy);
	fin();
	skel_xy_fin(x, y, skel);
}

/*

Put
git clean -fdxq
m clean lint
A=0.8835572001943658
f=data/rbc.skel
./2 area $A 1   bend_min 1e- 3 < $f

co.geomview -p cat *.skel

Kill git

*/
