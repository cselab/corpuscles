#include <stdio.h>
#include <tgmath.h>
#include <real.h>
#include <co/err.h>
#include <co/skel.h>

#define FMT CO_REAL_OUT

int
main(void)
{
	Skel *q;
	real *x, *y, length, b;
	int nv;

	b = 0.5;
	nv = 15;
	//skel_ellipse_ini(nv, b, &length, &x, &y, &q);
	skel_rbc_ini(nv, &length, &x, &y, &q);
	MSG("legnth " FMT, length);
	skel_edg_write(q, x, y, stdout);
	skel_xy_fin(x, y, q);
}
