#include <stdio.h>
#include <math.h>
#include <real.h>
#include <co/array.h>
#include <co/dlen.h>
#include <co/err.h>
#include <co/force2.h>
#include <co/macro.h>
#include <co/matrix.h>
#include <co/memory.h>
#include <co/oseen2.h>
#include <co/skel.h>
#include <co/punto.h>

#include <alg/linsolve.h>

#define FMT CO_REAL_OUT

static
Force2 *Force[99] =
{
	NULL
};
static Skel *skel;
static Oseen2 *oseen;
static real gdot = 1, mu = 1, dt = 5e-6;

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
	LinSolve *linsolve;
	real *x, *y, *vx, *vy, *fx, *fy;
	real *sigma, *rhs;
	real *Oxx, *Oxy, *Oyy;
	real *AOx, *AOy;
	real*Ax, *Ay, *res, *ser, *A;
	real xx, xy, yy;
	int n, i, j;
	int al, be, ga, de;
	real ax, ay, t, e;

	argv++;
	skel_read(stdin, &x, &y, &skel);
	fargv(&argv, skel);
	n = skel_nv(skel);
	e = 0.01;
	oseen2_ini(e, &oseen);

	CALLOC2(n, &vx, &vy);
	CALLOC2(n, &fx, &fy);
	MALLOC(n, &sigma);
	MALLOC(n, &rhs);
	CALLOC2(n, &res, &ser);
	matrix_ini(n, n, &Oxx);
	matrix_ini(n, n, &Oxy);
	matrix_ini(n, n, &Oyy);
	matrix_ini(n, n, &Ax);
	matrix_ini(n, n, &Ay);
	matrix_ini(n, n, &AOx);
	matrix_ini(n, n, &AOy);
	matrix_ini(n, n, &A);
	lin_solve_ini(n, &linsolve);

	for (j = 0; j < 10000; j++) {
		for (i = 0; i < n; i++) {
			vx[i] = gdot*y[i];
			vy[i] = 0;
		}
		array_zero(n, fx);
		array_zero(n, fy);
		force(skel, x, y, fx, fy);
		dlen_ver(skel, x, y, /**/ Ax, Ay);
		oseen2_apply(oseen, skel, x, y, Oxx, Oxy, Oyy);
		matrix_zero(n, n, A);
		array_zero(n, rhs);
		matrix_zero(n, n, AOx);
		matrix_zero(n, n, AOy);
		for (al = 0; al < n; al++)
			for (ga = 0; ga < n; ga++)
				for (be = 0; be < n; be++) {
					ax = matrix_get(n, n, al, be, Ax);
					ay = matrix_get(n, n, al, be, Ay);
					xx = matrix_get(n, n, be, ga, Oxx)/mu;
					xy = matrix_get(n, n, be, ga, Oxy)/mu;
					yy = matrix_get(n, n, be, ga, Oyy)/mu;
					matrix_add(n, n, al, ga, xx*ax + xy*ay, AOx);
					matrix_add(n, n, al ,ga, xy*ax + yy*ay, AOy);
				}
		for (al = 0; al < n; al++)
			for (de = 0; de < n; de++)
				for (ga = 0; ga < n; ga++) {
					xx = matrix_get(n, n, al, ga, AOx);
					yy = matrix_get(n, n, al, ga, AOy);
					ax = matrix_get(n, n, de, ga, Ax);
					ay = matrix_get(n, n, de, ga, Ay);
					t = xx*ax + yy*ay;
					matrix_add(n, n, al, de, t, A);
				}
		for (al = 0; al < n; al++)
			for (be = 0; be < n; be++) {
				xx = matrix_get(n, n, al, be, AOx);
				yy = matrix_get(n, n, al, be, AOy);
				ax = matrix_get(n, n, al, be, Ax);
				ay = matrix_get(n, n, al, be, Ay);
				t = ax*vx[be] + ay*vy[be]   + xx*fx[be] + yy*fy[be];
				rhs[al] -= t;
			}
		lin_solve_apply(linsolve, A, rhs, sigma);
		matrix_array_append_t(n, n, Ax, sigma, fx);
		matrix_array_append_t(n, n, Ay, sigma, fy);
		for (be = 0; be < n; be++)
			for (ga = 0; ga < n; ga++) {
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
		if (j % 1000 == 0) {
			MSG("x[0] " FMT, x[0]);
			sprintf(file, "%05d.skel", j);
			f = fopen(file, "w");
			skel_write(skel, x, y, f);
			fclose(f);
			const real *q[] = {x, y, vx, vy, sigma, fx, fy, NULL};
			punto_fwrite(n, q, stdout);
			printf("\n");
		}
	}

	MSG(FMT " " FMT, sigma[0],  sigma[n - 1]);
	//matrix_fwrite(n, 1, sigma, stdout);
	//matrix_fwrite(n, 1, res, stdout);

	lin_solve_fin(linsolve);
	FREE2(vx, vy);
	FREE2(fx, fy);
	FREE(sigma);
	FREE(rhs);
	matrix_fin(Oxx);
	matrix_fin(Oxy);
	matrix_fin(Oyy);
	matrix_fin(Ax);
	matrix_fin(Ay);
	matrix_fin(AOx);
	matrix_fin(AOy);
	FREE(res);
	FREE(ser);
	FREE(A);
	oseen2_fin(oseen);
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
