#include <stdio.h>
#include <tgmath.h>
#include <real.h>
#include <co/array.h>
#include <co/err.h>
#include <co/oseen2.h>
#include <co/skel.h>
#include <co/memory.h>
#include <co/matrix.h>

#define FMT CO_REAL_OUT

int
main(void)
{
	Skel *skel;
	real *x, *y;
	real *S, *Oxx, *Oxy, *Oyy;
	real xx, xy, yy, s;
	int n, i, j;

	skel_read(stdin, &x, &y, &skel);
	n = skel_nv(skel);

	matrix_ini(n , n, &S);
	matrix_ini(n, n, &Oxx);
	matrix_ini(n, n, &Oxy);
	matrix_ini(n, n, &Oyy);
	oseen2(skel, x, y, Oxx, Oxy, Oyy);

	for (i = 0; i < n; i++)
		for (j = 0;j < n; j++) {
		xx = matrix_get(n, n, i, j, Oxx);
		xy = matrix_get(n, n, i, j, Oxy);
		yy = matrix_get(n, n, i, j, Oyy);
		s = xx*yy - xy*xy;
		matrix_set(n, n, i, j, s, S);
	}
	
	for (i = 0; i < n; i++) {
		j = i + 1;
		if (j >= n)
			j -= n;
		printf(FMT "\n", matrix_get(n, n, i, j, S));
	}
	//matrix_fwrite(n, n, S, stdout);
	matrix_fin(S);
	matrix_fin(Oxx);
	matrix_fin(Oxy);
	matrix_fin(Oyy);
	skel_xy_fin(x, y, skel);
}
