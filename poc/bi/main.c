#include <stdio.h>
#include <tgmath.h>
#include <real.h>
#include <co/array.h>
#include <co/dlen.h>
#include <co/err.h>
#include <co/matrix.h>
#include <co/memory.h>
#include <co/oseen2.h>
#include <co/skel.h>

#define FMT CO_REAL_OUT

int
main(void)
{
	Skel *skel;
	real *x, *y, *vx, *vy, *ux, *uy;
	real *sigma;
	real *kx, *ky;
	real *Oxx, *Oxy, *Oyy;
	real*Ax, *Ay, *res, *ser, *A;
	real xx, xy, yy;
	int n, i, j;

	skel_read(stdin, &x, &y, &skel);
	n = skel_nv(skel);

	CALLOC2(n, &vx, &vy);
	CALLOC2(n, &ux, &uy);
	MALLOC(n, &sigma);
	CALLOC2(n, &kx, &ky);
	CALLOC2(n, &res, &ser);
	matrix_ini(n, n, &Oxx);
	matrix_ini(n, n, &Oxy);
	matrix_ini(n, n, &Oyy);
	matrix_ini(n, n, &Ax);
	matrix_ini(n, n, &Ay);
	matrix_ini(n, n, &A);

	dlen_ver(skel, x, y, /**/ Ax, Ay);
	array_one(n, sigma);
	matrix_array_append_n(n, n, Ax, sigma, kx);
	matrix_array_append_n(n, n, Ay, sigma, ky);
	oseen2(skel, x, y, Oxx, Oxy, Oyy);
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j ++) {
			xx = matrix_get(n, n, i, j, Oxx);
			xy = matrix_get(n, n, i, j, Oxy);
			yy = matrix_get(n, n, i, j, Oyy);
			vx[i] += xx*kx[j] + xy*ky[j];
			vy[i] += xy*kx[j] + yy*ky[j];
		}
	}
	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++) {
			xx = matrix_get(n, n, i, j, Ax);
			yy = matrix_get(n, n, i, j, Ay);
			res[i] += xx*vx[j] + yy*vy[j];
		}

	matrix_zero(n, n, A);
	int al, be, ga, de;
	real ax, ay, bx, by;
	for (al = 0; al < n; al++)
		for (de = 0; de < n; de++)
			for (ga = 0; ga < n; ga++)
				for (be = 0; be < n; be++) {
		ax = matrix_get(n, n, al, be, Ax);
		ay = matrix_get(n, n, al, be, Ay);
		xx = matrix_get(n, n, be, ga, Oxx);
		xy = matrix_get(n, n, be, ga, Oxy);
		yy = matrix_get(n, n, be, ga, Oyy);
		bx = matrix_get(n, n, de, ga, Ax);
		by = matrix_get(n, n, de, ga, Ay);
		matrix_set(n, n, al, de, xx*ax*bx + xy*ax*by + xy*ay*bx + yy*ay*by, A);
	}

	matrix_fwrite(n, n, res, stdout);

	FREE2(vx, vy);
	FREE2(ux, uy);
	FREE(sigma);
	FREE2(kx, ky);
	matrix_fin(Oxx);
	matrix_fin(Oxy);
	matrix_fin(Oyy);
	matrix_fin(Ax);
	matrix_fin(Ay);
	FREE(res);
	FREE(ser);
	FREE(A);

	MSG("n %d", n);
	skel_xy_fin(x, y, skel);
}
