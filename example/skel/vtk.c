#include <stdio.h>
#include <real.h>
#include <co/err.h>
#include <co/skel.h>

int
main(void)
{
	err_set_ignore();
	Skel *q;
	real *x, *y, length;
	int nv;
	nv = 20 ;
	skel_circle_ini(nv, &length, &x, &y, &q);
	skel_vtk_write(q, x, y, stdout);
	skel_xy_fin(x, y, q);
}
