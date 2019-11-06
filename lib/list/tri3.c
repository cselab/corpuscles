#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "real.h"
#include "co/err.h"
#include "co/he.h"
#include "co/macro.h"
#include "co/memory.h"
#include "co/tri.h"
#include "co/vec.h"

#include "co/list/c.h"
#include "co/list/tri3.h"

#define T Tri3List
#define FMT CO_REAL_OUT

enum {
    X, Y, Z
};

struct T {
    He *he;
    const real *x, *y, *z;
    int *tris;

    int ny, nz;
    real lo[3], hi[3], size;
    Clist *clist;
    int status;
    int tri;
    real point[3];
};

#define SIDE(n, D) \
	do { \
		l = hi[D] - lo[D]; \
		n = l / size; \
		if (n * size < l) n++; \
	} while (0)

int
tri3list_ini(const real lo[3], const real hi[3], real size, T ** pq)
{
    T *q;
    int nx, ny, nz;
    real l;
    Clist *clist;

    MALLOC(1, &q);

    SIDE(nx, X);
    SIDE(ny, Y);
    SIDE(nz, Z);

    clist_gen_nnn(nx, ny, nz, &clist);

    q->size = size;
    q->lo[X] = lo[X];
    q->lo[Y] = lo[Y];
    q->lo[Z] = lo[Z];

    q->hi[X] = hi[X];
    q->hi[Y] = hi[Y];
    q->hi[Z] = hi[Z];

    q->ny = ny;
    q->nz = nz;
    q->clist = clist;

    *pq = q;
    return CO_OK;
}

int
tri3list_fin(T * q)
{
    clist_fin(q->clist);
    FREE(q);
    return CO_OK;
}

static int
map(T * q, real x, real y, real z, int *i, int *j, int *k)
{
    real *lo, size;

    size = q->size;

    lo = q->lo;

    x -= lo[X];
    y -= lo[Y];
    z -= lo[Z];

    *i = x / size;
    *j = y / size;
    *k = z / size;
    return CO_OK;
}

static int
get3(T * q, int m, real a[3], real b[3], real c[3])
{
    const real *x, *y, *z;
    He *he;
    int i, j, k;

    he = q->he;
    x = q->x;
    y = q->y;
    z = q->z;
    he_tri_ijk(he, m, &i, &j, &k);
    vec_get(i, x, y, z, a);
    vec_get(j, x, y, z, b);
    vec_get(k, x, y, z, c);
    return CO_OK;
}

static int
cm(T * q, int m, /**/ real * u, real * v, real * w)
{
    real a[3], b[3], c[3], r[3];

    get3(q, m, a, b, c);
    tri_center(a, b, c, r);
    *u = r[X];
    *v = r[Y];
    *w = r[Z];
    return CO_OK;
}

#define IDX  (i*ny*nz + j*nz + k)
int
tri3list_push(T * q, He * he, const real * x, const real * y,
              const real * z)
{
    int i, j, k, m;
    int ny, nz, n;
    real u, v, w;

    clist_reset(q->clist);
    ny = q->ny;
    nz = q->nz;
    n = he_nt(he);

    q->he = he;
    q->x = x;
    q->y = y;
    q->z = z;

    for (m = 0; m < n; m++) {
        cm(q, m, &u, &v, &w);
        map(q, u, v, w, &i, &j, &k);
        if (clist_push(q->clist, IDX, m) != CO_OK) {
            MSG("ijk: %d %d %d", i, j, k);
            MSG("nyz: %d %d", ny, nz);
            ERR(CO_INDEX, "fail to push  traingle " FMT " " FMT " " FMT, u,
                v, w);
        }
    }
    return CO_OK;
}

static real
distance2(T * q, int m, const real p[3])
{
    real a[3], b[3], c[3];

    get3(q, m, a, b, c);
    return tri_point_distance2(a, b, c, p);
}

static int
closest(T * q, int m, const real p[3], real t[3])
{
    real a[3], b[3], c[3];

    get3(q, m, a, b, c);
    return tri_point_closest(a, b, c, p, t);
}

int
tri3list_get(T * q, real x, real y, real z)
{
    int i, j, k, ny, nz, status, tri, *tris, *tt;
    real r[3], d2, m2, *point, size;

    ny = q->ny;
    nz = q->nz;
    size = q->size;
    point = q->point;
    map(q, x, y, z, &i, &j, &k);
    if (clist_parts(q->clist, IDX, &tris) != CO_OK) {
        MSG("ijk: %d %d %d", i, j, k);
        MSG("nyz: %d %d", ny, nz);
        ERR(CO_INDEX, "clist_parts failed" FMT " " FMT " " FMT, x, y, z);
    }
    m2 = size * size;
    status = 0;
    tri = -1;
    vec_ini(x, y, z, r);
    tt = tris;
    while ((j = *tt++) != -1) {
        d2 = distance2(q, j, r);
        if (d2 < m2) {
            status = 1;
            m2 = d2;
            tri = j;
            closest(q, j, r, point);
        }
    }
    q->status = status;
    q->tri = tri;
    q->tris = tris;
    return CO_OK;
}


int
tri3list_status(T * q)
{
    return q->status;
}

int
tri3list_tri(T * q)
{
    return q->tri;
}

int
tri3list_tris(T * q, int **p)
{
    *p = q->tris;
    return CO_OK;
}

int
tri3list_point(T * q, real point[3])
{
    point[X] = q->point[X];
    point[Y] = q->point[Y];
    point[Z] = q->point[Z];
    return CO_OK;
}
