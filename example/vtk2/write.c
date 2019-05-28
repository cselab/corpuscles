#include <stdio.h>
#include <tgmath.h>
#include <real.h>
#include <co/err.h>
#include <co/vtk2.h>

#define FMT CO_REAL_OUT

int
main(void)
{
	Vtk2 *q;
	real lo[] = {1, 2}, hi[2] = {3, 4};
	int size[] = {20, 20};
	const real *ff[] = {NULL};
	const char *nn[] = {NULL};

	vtk2_ini(lo, hi, size, &q);

	vtk2_fwrite(q, ff, nn, stdout);
	vtk2_fin(q);
}
