#include <stdio.h>
#include <tgmath.h>

#include <real.h>
#include <co/array.h>
#include <co/len.h>
#include <co/err.h>
#include <co/force.h>
#include <co/macro.h>
#include <co/matrix.h>
#include <co/memory.h>
#include <co/oseen3.h>
#include <co/he.h>
#include <co/punto.h>

#define FMT CO_REAL_OUT

static
Force *Fo[99] =
{
	NULL
};
static He *he;
static Oseen3 *oseen;
static real gamma = 1, mu = 1, dt = 2e-5;

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
force(He *he, const real *x, const real *y, real *fx, real *fy)
{
	int i;
	i = 0;
	while (Fo[i]) {
		force_force(Fo[i], he, x, y, fx, fy);
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

int
main(__UNUSED int argc, char **argv)
{
	real *x, *y, *vx, *vy, *fx, *fy;
	real *Oxx, *Oxy, *Oyy;
	real xx, xy, yy, e;
	int n, i, j, k;
	int be, ga;

	argv++;
	he_read(stdin, &x, &y, &he);
	fargv(&argv, he);
	n = he_nv(he);
	e = 0.01;
	oseen3_ini(e, &oseen);
	MSG("len " FMT, len(he, x, y));
	CALLOC2(n, &vx, &vy);
	CALLOC2(n, &fx, &fy);
	matrix_ini(n, n, &Oxx);
	matrix_ini(n, n, &Oxy);
	matrix_ini(n, n, &Oyy);
	for (k = j = 0; j < 10000000; j++) {
		for (i = 0; i < n; i++) {
			vx[i] = gamma*y[i];
			vy[i] = 0;
		}
		array_zero(n, fx);
		array_zero(n, fy);
		force(he, x, y, fx, fy);
		oseen3_apply(oseen, he, x, y, Oxx, Oxy, Oyy);
		for (ga = 0; ga < n; ga++)
			for (be = 0; be < n; be++) {
				xx = matrix_get(n, n, be, ga, Oxx)/mu;
				xy = matrix_get(n, n, be, ga, Oxy)/mu;
				yy = matrix_get(n, n, be, ga, Oyy)/mu;
				vx[be] -= xx*fx[ga] + xy*fy[ga];
				vy[be] -= xy*fx[ga] + yy*fy[ga];
			}          

		FILE *f;
		char file[9999];
		for (i = 0; i < n; i++) {
			x[i] += dt*vx[i];
			y[i] += dt*vy[i];
		}
		if (j % 10000 == 0) {
			MSG("x[0] " FMT, x[0]);
			sprintf(file, "%05d.off", k++);
			f = fopen(file, "w");
			he_off_write(n, x, y, f);
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
	oseen3_fin(oseen);
	fin();
	he_xy_fin(x, y, he);
}

/*

Put

git clean -fdxq
m clean lint
A=0.8835572001943658
f=data/rbc.he
#./2d  len $f 1 0 0    bend_min 1e-3 < $f > q
./2d len $f 50 0.05 0.05 area $A 50     bend_min 0.5 < $f > q

co.geomview -f 38 -a /u/a *.off
punto -D 2 q
mpv p.mp4
           
Kill git

*/
        