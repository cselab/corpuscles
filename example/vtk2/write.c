#include <stdio.h>
#include <math.h>
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
	int n, i, j, k;
	real *f, *g;
	Vtk2 *q;
	real lo[] = {1, 2}, hi[2] = {5, 4};
	int size[] = {40, 20};

	n = size[X]*size[Y];
	CALLOC(n, &f);
	CALLOC(n, &g);

	for (i = 0; i < size[X]; i++)
		for (j = 0; j < size[Y]; j++) { 
			k = j*size[X] + i;
			f[k] = i;
			g[k] = j;
	}

	const real *ff[] = {f, g, NULL};
	const char *nn[] = {"f", "g", NULL};
	vtk2_ini(lo, hi, size, &q);
	vtk2_fwrite(q, ff, nn, stdout);
	vtk2_fin(q);
	FREE(f);
	FREE(g);
}
