#include <stdio.h>
#include <tgmath.h>
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
	real *Ax, *Ay;
	int n;

	skel_read(stdin, &x, &y, &skel);
	n = skel_nv(skel);
	MSG("len: " FMT, len(skel, x, y));

	matrix_ini(n, n, &Ax);
	matrix_ini(n, n, &Ay);
	dlen_ver(skel, x, y, Ax, Ay);
	
	matrix_fwrite(n, n, Ay, stdout);
	matrix_fin(Ax);
	matrix_fin(Ay);
	skel_xy_fin(x, y, skel);
}
