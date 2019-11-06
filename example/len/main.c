#include <stdio.h>
#include <math.h>
#include <real.h>
#include <co/array.h>
#include <co/err.h>
#include <co/len.h>
#include <co/skel.h>
#include <co/memory.h>

#define FMT CO_REAL_OUT

int
main(void)
{
	Skel *q;
	real *x, *y, *ver, *edg;
	int nv, ne;

	skel_read(stdin, &x, &y, &q);
	nv = skel_nv(q);
	ne = skel_ne(q);
	MALLOC(nv, &ver);
	MALLOC(ne, &edg);

	len_ver(q, x, y, ver);
	len_edg(q, x, y, edg);

	MSG("len: " FMT, len(q, x, y));
	MSG("ver: " FMT " " FMT " " FMT, array_min(nv, ver), array_mean(nv, ver), array_max(nv, ver));
	MSG("edg: " FMT " " FMT " " FMT, array_min(ne, edg), array_mean(ne, edg), array_max(ne, edg));

	FREE(ver);
	FREE(edg);
	skel_xy_fin(x, y, q);
}
