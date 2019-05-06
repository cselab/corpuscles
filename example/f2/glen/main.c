#include <stdio.h>
#include <real.h>
#include <co/err.h>
#include <co/f2/glen.h>
#include <co/memory.h>
#include <co/skel.h>
#include <co/punto.h>

#define FMT CO_REAL_OUT

int
main(int argc, char **argv)
{
	real *x, *y, *fx, *fy, eng, len;
	Skel *skel;
	int n;
	argv++;

	skel_read(stdin, &x, &y, &skel);
	F2Glen *force;

	n = skel_nv(skel);
	CALLOC(n, &fx);
	CALLOC(n, &fy);

	f2_glen_argv(&argv, skel, &force);
	eng = f2_glen_energy(force, skel, x, y);
	len = f2_glen_len(force);
	f2_glen_force(force, skel, x, y, fx, fy);
	MSG(FMT " " FMT "\n", eng, len);

	const real *q[] = {x, y, fx, fy, NULL};
	punto_fwrite(n, q, stdout);

	FREE(fx);
	FREE(fy);
	skel_xy_fin(x, y, skel);
	f2_glen_fin(force);
}