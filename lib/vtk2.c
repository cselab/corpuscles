#include <stdio.h>
#include "real.h"
#include "co/err.h"
#include "co/memory.h"
#include "co/vtk2.h"

#define T Vtk2
#define FMT CO_REAL_OUT

enum
{
	X, Y
};

struct T
{
	real origin[2], spacing[2];
	int n[2];
	double *f;
};

int
vtk2_ini(const real lo[2], const real hi[2], const int n[2], T **pq)
{
	T *q;
	int N;
	double *f;

	if (n[X] <= 1)
		ERR(CO_INDEX, "n[X]=%d <= 1", n[X]);
	if (n[Y] <= 1)
		ERR(CO_INDEX, "n[Y]=%d <= 1", n[Y]);
	if (lo[X] >= hi[X])
		ERR(CO_INDEX, "lo[X] >= hi[X]");
	if (lo[Y] >= hi[Y])
		ERR(CO_INDEX, "lo[Y] >= hi[Y]");
	N = n[X]*n[Y];
	MALLOC(1, &q);	
	MALLOC(N, &f);
	q->origin[X] = lo[X];
	q->origin[Y] = lo[Y];
	q->spacing[X] = (hi[X] - lo[X])/(n[X] - 1);
	q->spacing[Y] = (hi[Y] - lo[Y])/(n[Y] - 1);
	q->n[X] = n[X];
	q->n[Y] = n[Y];
	q->f = f;
	*pq = q;
	return CO_OK;
}

int
vtk2_fin(T *q)
{
	FREE(q->f);
	FREE(q);
	return CO_OK;
}

int
vtk2_fwrite(T *q, const real *field[], const char *name[], FILE *f)
{
	real *origin, *spacing;
	int *n, N, i, j;

	origin = q->origin;
	spacing = q->spacing;
	n = q->n;
	if (fputs("# vtk DataFile Version 2.0\n", f) == EOF)
		ERR(CO_IO, "fail to vtk data");
	fputs("fields\n", f);
	fputs("ASCII\n", f);
	fputs("DATASET STRUCTURED_POINTS\n", f);
	fprintf(f, "DIMENSIONS %d %d %d\n", n[X], n[Y], 1);
	fprintf(f, "ORIGIN " FMT " " FMT " 0\n", origin[X], origin[Y]);
	fprintf(f, "SPACING " FMT " " FMT " 0.0\n", spacing[X], spacing[Y]);	
	N = n[X]*n[Y];
	if (field[0] != NULL)
		fprintf(f, "POINT_DATA %d", N);
	for (i = 0; field[i] != NULL; i++) {
		fprintf(f, "SCALARS %s double 1\n", name[i]);
		for (j = 0; j < N; j++)
			fprintf(f, FMT "\n", field[i][j]);
	}
	return CO_OK;
}
