#include <stdio.h>
#include <real.h>

#include <co/he.h>
#include <co/y.h>
#include <co/bbox.h>

#define FMT CO_REAL_OUT

enum
{
	X, Y, Z
};

int
main(int argc, char **argv)
{
	Bbox *bbox;
	He *he;
	real *lo, *hi, *x, *y, *z;
	int n;

	y_inif(stdin, &he, &x, &y, &z);
	bbox_ini(&bbox);
	n = he_nv(he);
	bbox_update(bbox, n, x, y, z);

	bbox_lo(bbox, &lo);
	bbox_hi(bbox, &hi);

	printf(FMT " " FMT " " FMT "\n", lo[X], lo[Y], lo[Z]);
	printf(FMT " " FMT " " FMT "\n", hi[X], hi[Y], hi[Z]);

	bbox_fin(bbox);
	y_fin(he, x, y, z);

	return 0;
}
