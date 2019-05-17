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
	real gamma;
	Skel *skel;
	real *x, *y, *vx, *vy, *ux, *uy, *fx, *fy;
	real *sigma, *rhs;
	real *kx, *ky;
	real *Oxx, *Oxy, *Oyy;
	real *OAx, *OAy;
	real*Ax, *Ay, *res, *ser, *A;
	real xx, xy, yy;
	int n, i, j;
	int al, be, ga, de;
	real ax, ay, t;

	skel_read(stdin, &x, &y, &skel);
	n = skel_nv(skel);

	CALLOC2(n, &vx, &vy);
	MALLOC2(n, &ux, &uy);
	MALLOC2(n, &fx, &fy);
	MALLOC(n, &sigma);
	CALLOC(n, &rhs);
	MALLOC2(n, &kx, &ky);
	CALLOC2(n, &res, &ser);
	matrix_ini(n, n, &Oxx);
	matrix_ini(n, n, &Oxy);
	matrix_ini(n, n, &Oyy);
	matrix_ini(n, n, &Ax);
	matrix_ini(n, n, &Ay);
	matrix_ini(n, n, &OAx);
	matrix_ini(n, n, &OAy);
	matrix_ini(n, n, &A);

	gamma = 1;
	for (i = 0; i < n; i++) {
		ux[i] = gamma*y[i];
		uy[i] = 0;
	}
	for (i = 0; i < n; i++) {
		fx[i] = y[i];
		fy[i] = x[i];
	}

	dlen_ver(skel, x, y, /**/ Ax, Ay);
	array_one(n, sigma);
	oseen2(skel, x, y, Oxx, Oxy, Oyy);

	for (i = 0; i < n; i++) {
		kx[i] += fx[i];
		ky[i] += fy[i];
	}
	matrix_array_append_t(n, n, Ax, sigma, kx);
	matrix_array_append_t(n, n, Ay, sigma, ky);
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

	matrix_zero(n, n, A);
	matrix_zero(n, n, OAx);
	matrix_zero(n, n, OAy);
	for (al = 0; al < n; al++)
		for (ga = 0; ga < n; ga++)
			for (be = 0; be < n; be++) {
		xx = matrix_get(n, n, be, ga, Oxx);
		xy = matrix_get(n, n, be, ga, Oxy);
		yy = matrix_get(n, n, be, ga, Oyy);
		ax = matrix_get(n, n, al, be, Ax);
		ay = matrix_get(n, n, al, be, Ay);
		matrix_add(n, n, al, ga, xx*ax + xy*ay, OAx);
		matrix_add(n, n, al ,ga, xy*ax + yy*ay, OAy);
	}
	for (al = 0; al < n; al++)
		for (de = 0; de < n; de++)
			for (ga = 0; ga < n; ga++) {
		xx = matrix_get(n, n, al, ga, OAx);
		yy = matrix_get(n, n, al, ga, OAy);
		ax = matrix_get(n, n, de, ga, Ax);
		ay = matrix_get(n, n, de, ga, Ay);
		t = xx*ax + yy*ay;
		matrix_add(n, n, al, de, t, A);
	}
	for (al = 0; al < n; al++)
		for (be = 0; be < n; be++) {
		xx = matrix_get(n, n, al, be, OAx);
		yy = matrix_get(n, n, al, be, OAy);
		ax = matrix_get(n, n, al, be, Ax);
		ay = matrix_get(n, n, al, be, Ay);
		t = ax*ux[be] + ay*uy[be]   + xx*fx[be] + yy*fy[be];
		rhs[al] += t;
	}

	matrix_array_n(n, n, A, sigma, ser);
	for (i = 0; i < n; i++)
		ser[i] += rhs[i];

	MSG(FMT " " FMT, ser[0],  res[0]);
	MSG(FMT " " FMT, ser[n - 1],  res[n - 1]);	
	matrix_fwrite(n, 1, ser, stdout);
	//matrix_fwrite(n, 1, res, stdout);

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
	matrix_fin(OAx);
	matrix_fin(OAy);
	FREE(res);
	FREE(ser);
	FREE(A);
	skel_xy_fin(x, y, skel);
}
