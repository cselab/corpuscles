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
	real *x, *y, length;
	int nv;
	nv = 15;
	skel_rbc_ini(nv, &length, &x, &y, &q);
	MSG("legnth " FMT, length);
	skel_edg_color_write(q, x, y, x, stdout);	
	skel_xy_fin(x, y, q);
}
