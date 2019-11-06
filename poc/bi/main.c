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

#include <alg/linsolve.h>

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

int
main(__UNUSED int argc, char **argv)
{
	LinSolve *linsolve;
	real gdot;
	Skel *skel;
	Oseen2 *oseen;
	real *x, *y, *vx, *vy, *ux, *uy, *fx, *fy;
	real *sigma, *rhs;
	real *kx, *ky;
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
	MALLOC2(n, &ux, &uy);
	CALLOC2(n, &fx, &fy);
	MALLOC(n, &sigma);
	CALLOC(n, &rhs);
	CALLOC2(n, &kx, &ky);
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

	gdot = 1;
	for (i = 0; i < n; i++) {
		ux[i] = gdot*y[i];
		uy[i] = 0;
	}
	force(skel, x, y, fx, fy);

	dlen_ver(skel, x, y, /**/ Ax, Ay);
	array_one(n, sigma);
	oseen2_apply(oseen, skel, x, y, Oxx, Oxy, Oyy);

	for (i = 0; i < n; i++) {
		kx[i] += fx[i];
		ky[i] += fy[i];
	}
	matrix_zero(n, n, A);
	matrix_zero(n, n, AOx);
	matrix_zero(n, n, AOy);
	for (al = 0; al < n; al++)
		for (ga = 0; ga < n; ga++)
			for (be = 0; be < n; be++) {
				xx = matrix_get(n, n, be, ga, Oxx);
				xy = matrix_get(n, n, be, ga, Oxy);
				yy = matrix_get(n, n, be, ga, Oyy);
				ax = matrix_get(n, n, al, be, Ax);
				ay = matrix_get(n, n, al, be, Ay);
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
			t = ax*ux[be] + ay*uy[be]   + xx*fx[be] + yy*fy[be];
			rhs[al] += t;
		}
	lin_solve_apply(linsolve, A, rhs, sigma);

	matrix_array_substr_t(n, n, Ax, sigma, kx);
	matrix_array_substr_t(n, n, Ay, sigma, ky);
	for (be = 0; be < n; be++) {
		for (ga = 0; ga < n; ga ++) {
			xx = matrix_get(n, n, be, ga, Oxx);
			xy = matrix_get(n, n, be, ga, Oxy);
			yy = matrix_get(n, n, be, ga, Oyy);
			vx[be] += xx*kx[ga] + xy*ky[ga];
			vy[be] += xy*kx[ga] + yy*ky[ga];
		}
		vx[be] += ux[be];
		vy[be] += uy[be];
	}
	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++) {
			xx = matrix_get(n, n, i, j, Ax);
			yy = matrix_get(n, n, i, j, Ay);
			res[i] += xx*vx[j] + yy*vy[j];
		}

	matrix_array_n(n, n, A, sigma, ser);
	for (i = 0; i < n; i++)
		ser[i] -= rhs[i];

	MSG(FMT " " FMT, ser[0],  res[0]);
	MSG(FMT " " FMT, ser[n - 1],  res[n - 1]);
	matrix_fwrite(n, 1, sigma, stdout);
	//matrix_fwrite(n, 1, res, stdout);

	lin_solve_fin(linsolve);
	FREE2(vx, vy);
	FREE2(ux, uy);
	FREE2(fx, fy);
	FREE(sigma);
	FREE(rhs);
	FREE2(kx, ky);
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
	fin();
	oseen2_fin(oseen);
	skel_xy_fin(x, y, skel);
}

/*

m clean lint
A=0.8835572001943658
f=data/rbc.skel
valgrind '--track-origins=yes' ./main len $f 1 0 0    area $A 1     bend_min 1e-3 < $f > q

*/
