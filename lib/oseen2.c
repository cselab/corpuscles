#include <tgmath.h>
#include <stdio.h>
#include "real.h"
#include "co/err.h"

#include "co/skel.h"
#include "co/oseen2.h"
#include "co/vec2.h"
#include "co/matrix.h"

static const real r0 = 0.1;

static int
oseen(const real a[2], const real b[2], real *xx, real *xy, real *yy)
{
	enum {X, Y};
	real d[2], r, r2,l;

	vec2_minus(a, b, d);
	r = vec2_abs(d);
	if (r == 0)
		ERR(CO_NUM, "r == 0");
	r2 = r*r;
	l = log(r/r0);
	*xx = -l + d[X]*d[X]/r2;
	*xy = d[X]*d[Y]/r2;
	*yy = -l + d[Y]*d[Y]/r2;
	return CO_OK;
}

int
oseen2(Skel *skel, const real *x, const real *y, real *xx, real *xy, real *yy)
{
	int n, i, j;
	real a[2], b[2], oxx,oxy, oyy;
	n = skel_nv(skel);

	for (i = 0; i < n; i++) {
		vec2_get(i, x, y, a);
		matrix_set(n, n, i, i, 0, xx);
		matrix_set(n, n, i, i, 0, xy);
		matrix_set(n, n, i, i, 0, yy);
		for (j = 0; j < n; j++) {
			if (i == j) continue;
			vec2_get(j, x, y, b);
			oseen(a, b, &oxx, &oxy, &oyy);
			matrix_set(n, n, i, j, oxx, xx);
			matrix_set(n, n, i, j, oxy, xy);
			matrix_set(n, n, i, j, oyy, yy);
		}
	}			
	return CO_OK;
}
