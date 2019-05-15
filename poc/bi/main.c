#include <stdio.h>
#include <tgmath.h>
#include <real.h>
#include <co/err.h>
#include <co/memory.h>
#include <co/matrix.h>
#include <co/skel.h>

#define FMT CO_REAL_OUT

int
main(void)
{
	Skel *q;
	real *x, *y, *vx, *vy, *ux, *uy;
	real *Ax, *Ay, *sigma;
	real *kx, *ky;
	real *Oxx, *Oxy, *Oyy;

	int n;
	skel_read(stdin, &x, &y, &q);
	n = skel_nv(q);
	
	MALLOC2(n, &vx, &vy);
	MALLOC2(n, &ux, &uy);
	MALLOC2(n, &Ax, &Ay);
	MALLOC(n, &sigma);
	MALLOC2(n, &kx, &ky);
	MALLOC3(n, &Oxx, &Oxy, &Oyy);

	FREE2(vx, vy);
	FREE2(ux, uy);
	FREE2(Ax, Ay);
	FREE(sigma);
	FREE2(kx, ky);
	FREE3(Oxx, Oxy, Oyy);

	MSG("n %d", n);
	skel_xy_fin(x, y, q);
}
