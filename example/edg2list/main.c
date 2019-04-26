#include <stdio.h>

#include <real.h>

#include <co/array.h>
#include <co/bbox.h>
#include <co/edg.h>
#include <co/err.h>
#include <co/skel.h>
#include <co/list/edg2.h>
#include <co/macro.h>
#include <co/memory.h>
#include <co/vec2.h>
#include <co/off.h>
#include <co/y.h>

#define FMT CO_REAL_OUT
enum 
{
	X, Y
};

int
main(__UNUSED int argc, const char **argv)
{
	Skel *skel;
	Edg2List *list;
	Bbox *bbox;
	real *x, *y, *color;
	real *lo, *hi;
	real size = 0.1;
	real p[2];
	real point[2], u[2];
	int status, ne, nv, t, j, *tris;

	argv++;
	vec2_argv(&argv, p);
	skel_read(stdin,  &x, &y, &skel);
	nv = skel_nv(skel);
	ne = skel_ne(skel);
	bbox_ini(&bbox);
	bbox_update(bbox, nv, x, y, z);
	bbox_lo(bbox, &lo);
	bbox_hi(bbox, &hi);

	vec2_ini(size, size, u);
	vec2_add(u, hi);
	vec2_sub(u, lo);

	CALLOC(nt, &color);

	edg2list_ini(lo, hi, size, &list);
	edg2list_push(list, skel, x, y, z);

	edg2list_get(list, p[X], p[Y], p[Z]);
	status = edg2list_status(list);
	MSG("status: %d", status);
	if (status) {
		edg2list_tris(list, &tris);
		while ( (j = *tris++) != -1)
			color[j] = 1;
		t = edg2list_tri(list);
		color[t] = 2;
		edg2list_point(list, point);
		fputs("LIST{\n", stdout);
		edg_vect(p, point, stdout);
		//boff_tri_fwrite(he, x, y, z, color, stdout);
		fputs("}", stdout);
	}

	FREE(color);
	edg2list_fin(list);
	bbox_fin(bbox);
	y_fin(he, x, y, z);
}

