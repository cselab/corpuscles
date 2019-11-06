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
	real *x, *y, length;
	int nv;
	nv = 40;
	skel_circle_ini(nv, &length, &x, &y, &q);
	MSG("legnth " FMT, length);
	skel_write(q, x, y, stdout);	
	skel_xy_fin(x, y, q);
}
