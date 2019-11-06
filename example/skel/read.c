#include <stdio.h>
#include <math.h>
#include <real.h>
#include <co/err.h>
#include <co/skel.h>

#define FMT CO_REAL_OUT

int
main(void)
{
	Skel *q;
	real *x, *y;
	int nv, ne;

	skel_read(stdin, &x, &y, &q);
	nv = skel_nv(q);
	ne = skel_ne(q);
	MSG("ve %d %d", nv, ne);
	skel_off_write(nv, x, y, stdout);
	skel_xy_fin(x, y, q);
}
