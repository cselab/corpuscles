#include <stdio.h>
#include <tgmath.h>
#include <real.h>
#include <co/err.h>
#include <co/vtk2.h>
#include <co/memory.h>

#define FMT CO_REAL_OUT

enum
{
	X, Y
};

int
main(void)
{
	int n;
	real *f;
	Vtk2 *q;
	real lo[] = {1, 2}, hi[2] = {3, 4};
	int size[] = {20, 20};

	n = size[X]*size[Y];
	CALLOC(n, &f);

	const real *ff[] = {f, NULL};
	const char *nn[] = {"f", NULL};
	vtk2_ini(lo, hi, size, &q);
	vtk2_fwrite(q, ff, nn, stdout);
	vtk2_fin(q);
	FREE(f);
}
