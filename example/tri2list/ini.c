#include <stdio.h>

#include <real.h>

#include <co/array.h>
#include <co/bbox.h>
#include <co/edg.h>
#include <co/err.h>
#include <co/he.h>
#include <co/list/tri2.h>
#include <co/macro.h>
#include <co/memory.h>
#include <co/vec.h>
#include <co/off.h>
#include <co/y.h>

#define FMT CO_REAL_OUT

int
main(__UNUSED int argc, const char **argv)
{
    He *he;
    Tri2List *list;
    Bbox *bbox;
    real *x, *y, *z;
    real *lo, *hi;
    real size = 0.1;
    real p[3];
    int nv;

    argv++;
    vec_argv(&argv, p);
    y_inif(stdin, &he, &x, &y, &z);
    nv = he_nv(he);
    bbox_ini(&bbox);
    bbox_update(bbox, nv, x, y, z);
    bbox_lo(bbox, &lo);
    bbox_hi(bbox, &hi);
    tri2list_ini(lo, hi, size, &list);
    tri2list_fin(list);
    bbox_fin(bbox);
    y_fin(he, x, y, z);
}
