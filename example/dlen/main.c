#include <stdio.h>
#include <math.h>
#include <real.h>
#include <co/array.h>
#include <co/err.h>
#include <co/len.h>
#include <co/dlen.h>
#include <co/skel.h>
#include <co/memory.h>
#include <co/matrix.h>

#define FMT CO_REAL_OUT

int
main(void)
{
	Skel *skel;
	real *x, *y;
	real *Ax, *Ay, *S;
	real ax, ay, s;
	int n, i, j;

	skel_read(stdin, &x, &y, &skel);
	n = skel_nv(skel);
	MSG("len: " FMT, len(skel, x, y));

	matrix_ini(n , n, &S);
	matrix_ini(n, n, &Ax);
	matrix_ini(n, n, &Ay);
	dlen_ver(skel, x, y, Ax, Ay);

	for (i = 0; i < n; i++)
		for (j = 0;j < n; j++) {
		ax = matrix_get(n, n, i, j, Ax);
		ay = matrix_get(n, n, i, j, Ay);
		s = ax*ax + ay*ay;
		matrix_set(n, n, i, j, s, S);
	}
	
	matrix_fwrite(n, n, S, stdout);
	matrix_fin(S);
	matrix_fin(Ax);
	matrix_fin(Ay);
	skel_xy_fin(x, y, skel);
}
