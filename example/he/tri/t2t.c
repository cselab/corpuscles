#include <stdio.h>

#include <real.h>
#include <co/distmesh.h>
#include <co/err.h>
#include <co/he.h>
#include <co/memory.h>
#include <co/y.h>

int
main()
{
	He *he;
	real *x, *y, *z;
	int i, j, k, m, t, nt;
	int *t2t;

	y_inif(stdin, &he, &x, &y, &z);
	nt = he_nt(he);
	MALLOC(3 * nt, &t2t);
	distmesh_t2t(he, t2t);
	for (t = m = 0; t < nt; t++) {
		i = t2t[m++];
		j = t2t[m++];
		k = t2t[m++];
		printf("%d %d %d\n", i, j, k);
	}
	FREE(t2t);
	y_fin(he, x, y, z);
	return CO_OK;
}
