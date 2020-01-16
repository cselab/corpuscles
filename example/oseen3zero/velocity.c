#include <stdio.h>
#include <math.h>
#include <real.h>
#include <co/array.h>
#include <co/err.h>
#include <co/he.h>
#include <co/matrix.h>
#include <co/memory.h>
#include <co/oseen3zero.h>
#include <co/y.h>

#define FMT CO_REAL_OUT

int
main(void)
{
	He *he;
	Oseen3Zero *oseen;
	real *x, *y, *z;
	real *S, *Oxx, *Oxy, *Oxz, *Oyy, *Oyz, *Ozz;
	real xx, yy, zz, s;
	int n, nt, i, j, k, t;

	err_set_ignore();
	y_inif(stdin, &he, &x, &y, &z);
	n = he_nv(he);
	nt = he_nt(he);

	oseen3_zero_ini(he, &oseen);
	oseen3_zero_fin(oseen);
	y_fin(he, x, y, z);
}
