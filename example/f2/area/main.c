#include <stdio.h>
#include <real.h>
#include <co/err.h>
#include <co/f2/area.h>
#include <co/memory.h>
#include <co/skel.h>
#include <co/punto.h>

#define FMT CO_REAL_OUT

int
main(int argc, char **argv)
{
	real *x, *y, *fx, *fy, eng, area;
	Skel *skel;
	int n;
	argv++;

	skel_read(stdin, &x, &y, &skel);
	F2Area *force;

	n = skel_nv(skel);
	CALLOC(n, &fx);
	CALLOC(n, &fy);

	f2_area_argv(&argv, skel, &force);
	eng = f2_area_energy(force, skel, x, y);
	area = f2_area_area(force);
	f2_area_force(force, skel, x, y, fx, fy);
	MSG(FMT " " FMT, eng, area);

	const real *q[] = {x, y, fx, fy, NULL};
	punto_fwrite(n, q, stdout);

	FREE(fx);
	FREE(fy);
	skel_xy_fin(x, y, skel);
	f2_area_fin(force);
}