#include <tgmath.h>
#include <stdio.h>
#include "real.h"
#include "co/array.h"
#include "co/edg2.h"
#include "co/err.h"

#include "co/skel.h"
#include "co/oseen2.h"
#include "co/vec2.h"

int
oseen2(Skel *skel, const real *x, const real *y, real *xx, real *xy, real *yy)
{
	int n, i, j;
	n = skel_nv(skel);

	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++)
			
	return CO_OK;
}
