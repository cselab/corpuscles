#include <stdio.h>

#include <real.h>

#include <co/array.h>
#include <co/bbox2.h>
#include <co/edg2.h>
#include <co/err.h>
#include <co/skel.h>
#include <co/list/edg1.h>
#include <co/macro.h>
#include <co/memory.h>
#include <co/vec2.h>
#include <co/off.h>
#include <co/y.h>

#define FMT CO_REAL_OUT
enum
{
	X
};

int main(__UNUSED int argc, const char **argv)
{
	Skel *skel;
	Edg1List *list;
	Bbox2 *bbox;
	real *x, *y, *color;
	real *lo, *hi;
	real size = 0.1;
	real p[2], u[2];
	int ne, nv, j, *edgs;

	argv++;
	vec2_argv(&argv, p);
	skel_read(stdin, &x, &y, &skel);
	ne = skel_ne(skel);
	nv = skel_nv(skel);
	bbox2_ini(&bbox);
	bbox2_update(bbox, nv, x, y);
	bbox2_lo(bbox, &lo);
	bbox2_hi(bbox, &hi);
	vec2_ini(size, size, u);
	vec2_add(u, hi);
	vec2_sub(u, lo);
	CALLOC(ne, &color);
	edg1list_ini(lo[X], hi[X], size, &list);
	edg1list_push(list, skel, x);
	edg1list_edgs(list, p[X], &edgs);
	while ( (j = *edgs++) != -1)
		color[j] = 1;
	fputs("LIST{\n", stdout);
	edg2_vect(p, p, stdout);
	skel_edg_color_write(skel, x, y, color, stdout);
	fputs("}", stdout);

	FREE(color);
	edg1list_fin(list);
	bbox2_fin(bbox);
	skel_xy_fin(x, y, skel);
}

