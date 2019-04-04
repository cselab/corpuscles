#include <stdio.h>

#include <real.h>
#include <co/array.h>
#include <co/bbox.h>
#include <co/edg.h>
#include <co/err.h>
#include <co/he.h>
#include <co/list/tri3.h>
#include <co/memory.h>
#include <co/vec.h>
#include <co/off.h>
#include <co/y.h>

#define FMT CO_REAL_OUT
enum 
{
	X, Y, Z
};

int main(void)
{
	He *he;
	Tri3List *list;
	Bbox *bbox;
	real *x, *y, *z, *color;
	real *lo, *hi;
	real size = 0.5;
	real p[3] = {0.05, 1.03, 0.1};
	real point[3], u[3];
	int status, nt, nv, t;

	y_inif(stdin, &he, &x, &y, &z);
	nt = he_nt(he);
	nv = he_nv(he);
	bbox_ini(&bbox);
	bbox_update(bbox, nv, x, y, z);
	bbox_lo(bbox, &lo);
	bbox_hi(bbox, &hi);

	vec_ini(size, size, size, u);
	vec_add(u, hi);
	vec_sub(u, lo);	

	CALLOC(nt, &color);

	tri3list_ini(lo, hi, size, &list);
	tri3list_push(list, he, x, y, z);

	tri3list_get(list, p[X], p[Y], p[Z]);
	status = tri3list_status(list);
	MSG("status: %d", status);
	if (status) {
		t = tri3list_tri(list);
		color[t] = 1;
		tri3list_point(list, point);
		fputs("LIST{\n", stdout);
		edg_vect(p, point, stdout);
		boff_tri_fwrite(he, x, y, z, color, stdout);
		fputs("}", stdout);
	}

	FREE(color);
	tri3list_fin(list);
	bbox_fin(bbox);
	y_fin(he, x, y, z);
}

