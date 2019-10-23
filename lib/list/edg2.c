#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>

#include "real.h"
#include "co/edg2.h"
#include "co/err.h"
#include "co/list/c.h"
#include "co/list/edg2.h"
#include "co/macro.h"
#include "co/memory.h"
#include "co/skel.h"
#include "co/vec2.h"

#define T Edg2List
#define FMT CO_REAL_OUT

enum {
    X, Y
};

struct T {
    Skel *skel;
    const real *x, *y;
    int ny;
    real lo[2], hi[2], size;
    Clist *clist;
    int status;
    int edg, *edgs;
    real point[2];
};

#define SIDE(n, D) \
	do { \
		l = hi[D] - lo[D]; \
		n = l / size; \
		if (n * size < l) n++; \
	} while (0)

int
edg2list_ini(const real lo[2], const real hi[2], real size, T ** pq)
{
    T *q;
    int nx, ny;
    real l;
    Clist *clist;

    MALLOC(1, &q);

    SIDE(nx, X);
    SIDE(ny, Y);

    clist_gen_nn(nx, ny, &clist);

    q->size = size;
    q->lo[X] = lo[X];
    q->lo[Y] = lo[Y];

    q->hi[X] = hi[X];
    q->hi[Y] = hi[Y];

    q->ny = ny;
    q->clist = clist;

    *pq = q;
    return CO_OK;
}

int
edg2list_fin(T * q)
{
    clist_fin(q->clist);
    FREE(q);
    return CO_OK;
}

static int
map(T * q, real x, real y, int *i, int *j)
{
    real *lo, size;

    size = q->size;
    lo = q->lo;
    x -= lo[X];
    y -= lo[Y];
    *i = x / size;
    *j = y / size;
    return CO_OK;
}

static int
get2(T * q, int m, real a[2], real b[2])
{
    const real *x, *y;
    Skel *skel;
    int i, j;

    skel = q->skel;
    x = q->x;
    y = q->y;
    skel_edg_ij(skel, m, &i, &j);

    vec2_get(i, x, y, a);
    vec2_get(j, x, y, b);

    return CO_OK;
}

static int
cm(T * q, int m, /**/ real * u, real * v)
{
    real a[2], b[2];

    get2(q, m, a, b);

    *u = (a[X] + b[X]) / 2;
    *v = (a[Y] + b[Y]) / 2;
    return CO_OK;
}

#define IDX  (i*ny + j)
int
edg2list_push(T * q, Skel * skel, const real * x, const real * y)
{
    int i, j, m;
    int ny, n;
    real u, v;

    clist_reset(q->clist);
    ny = q->ny;
    n = skel_ne(skel);

    q->skel = skel;
    q->x = x;
    q->y = y;

    for (m = 0; m < n; m++) {
        cm(q, m, &u, &v);
        map(q, u, v, &i, &j);
        if (clist_push(q->clist, IDX, m) != CO_OK) {
            MSG("ij %d %d", i, j);
            MSG("ny %d", ny);
            ERR(CO_INDEX, "fail to push  edge " FMT " " FMT, u, v);
        }
    }
    return CO_OK;
}

static real
distance2(T * q, int m, const real p[2])
{
    real a[2], b[2];

    get2(q, m, a, b);
    return edg2_point_distance2(a, b, p);
}

static int
closest(T * q, int m, const real p[2], real t[2])
{
    real a[2], b[2];

    get2(q, m, a, b);
    return edg2_point_closest(a, b, p, t);
}

int
edg2list_get(T * q, real x, real y)
{
    int i, j, ny, status, edg, *edgs, *ee;
    real r[2], d2, m2, *point, size;

    ny = q->ny;
    size = q->size;
    point = q->point;
    map(q, x, y, &i, &j);
    if (clist_parts(q->clist, IDX, &edgs) != CO_OK) {
        MSG("ij %d %d", i, j);
        MSG("ny %d", ny);
        ERR(CO_INDEX, "clist_parts failed" FMT " " FMT, x, y);
    }
    m2 = size * size;
    status = 0;
    edg = -1;
    vec2_ini(x, y, r);
    ee = edgs;
    while ((j = *ee++) != -1) {
        d2 = distance2(q, j, r);
        if (d2 < m2) {
            status = 1;
            m2 = d2;
            edg = j;
            closest(q, j, r, point);
        }
    }
    q->edgs = edgs;
    q->status = status;
    q->edg = edg;
    return CO_OK;
}


int
edg2list_status(T * q)
{
    return q->status;
}

int
edg2list_edg(T * q)
{
    return q->edg;
}

int
edg2list_point(T * q, real point[2])
{
    point[X] = q->point[X];
    point[Y] = q->point[Y];
    return CO_OK;
}

int
edg2list_edgs(T * q, int **edgs)
{
    *edgs = q->edgs;
    return CO_OK;
}
