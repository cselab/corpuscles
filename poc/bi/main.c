#include <stdio.h>
#include <tgmath.h>
#include <real.h>
#include <co/array.h>
#include <co/dlen.h>
#include <co/err.h>
#include <co/memory.h>
#include <co/matrix.h>
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
	real*Ax, *Ay;

	int n;
	skel_read(stdin, &x, &y, &skel);
	n = skel_nv(skel);
	
	MALLOC2(n, &vx, &vy);
	MALLOC2(n, &ux, &uy);
	MALLOC(n, &sigma);
	MALLOC2(n, &kx, &ky);
	matrix_ini(n, n, &Oxx);
	matrix_ini(n, n, &Oxy);
	matrix_ini(n, n, &Oyy);
	matrix_ini(n, n, &Ax);
	matrix_ini(n, n, &Ay);

	dlen_ver(skel, x, y, /**/ Ax, Ay);
	array_one(n, sigma);

	FREE2(vx, vy);
	FREE2(ux, uy);
	FREE(sigma);
	FREE2(kx, ky);
	matrix_fin(Oxx);
	matrix_fin(Oxy);
	matrix_fin(Oyy);
	matrix_fin(Ax);
	matrix_fin(Ay);

	MSG("n %d", n);
	skel_xy_fin(x, y, skel);
}
