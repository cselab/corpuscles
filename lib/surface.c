#include <stdio.h>
#include <math.h>
#include "real.h"

#include "co/bbox.h"
#include "co/err.h"
#include "co/he.h"
#include "co/list/tri2.h"
#include "co/memory.h"
#include "co/predicate.h"
#include "co/tri.h"
#include "co/vec.h"

#include "co/surface.h"

enum {
    X, Y, Z
};

#define T Surface
struct T {
    Bbox *bbox;
    He *he;
    Tri2List *list;
    const real *x, *y, *z;
};

int
surface_ini(real lo[2], real hi[2], real size, T ** pq)
{
    T *q;

    MALLOC(1, &q);
    bbox_ini(&q->bbox);
    tri2list_ini(lo, hi, size, &q->list);
    predicate_ini();
    *pq = q;
    return CO_OK;
}

int
surface_fin(T * q)
{
    bbox_fin(q->bbox);
    tri2list_fin(q->list);
    FREE(q);
    return CO_OK;
}

int
surface_update(T * q, He * he, const real * x, const real * y,
               const real * z)
{
    int n;

    q->he = he;
    q->x = x;
    q->y = y;
    q->z = z;
    n = he_nv(he);
    bbox_update(q->bbox, n, x, y, z);
    tri2list_push(q->list, he, x, y);
    return CO_OK;
}

#define max(a, b) ( (a) > (b) ? (a) : (b) )
int
surface_inside(T * q, real u, real v, real w)
{
    int n, t, i, j, k, m;
    He *he;
    Bbox *bbox;
    const real *x, *y, *z;
    real a[3], b[3], c[3], d[3], e[3];
    real zm, eps;

    eps = 1e-10;
    he = q->he;
    x = q->x;
    y = q->y;
    z = q->z;
    bbox = q->bbox;
    vec_ini(u, v, w, /**/ d);
    zm = bbox_zhi(bbox);
    vec_ini(u, v, max(zm, w) + eps, /**/ e);
    n = he_nt(he);
    for (t = m = 0; t < n; t++) {
        he_tri_ijk(he, t, &i, &j, &k);
        vec_get(i, x, y, z, a);
        vec_get(j, x, y, z, b);
        vec_get(k, x, y, z, c);
        m += predicate_ray(d, e, a, b, c);
    }
    return m % 2;
}

#define max(a, b) ( (a) > (b) ? (a) : (b) )
int
surface_inside_fast(T * q, real u, real v, real w)
{
    int t, i, j, k, m;
    He *he;
    Bbox *bbox;
    const real *x, *y, *z;
    real a[3], b[3], c[3], d[3], e[3];
    real zm, eps;
    int *tris;

    eps = 1e-10;
    he = q->he;
    x = q->x;
    y = q->y;
    z = q->z;
    bbox = q->bbox;
    if (!bbox_inside(bbox, u, v, w))
        return 0;

    vec_ini(u, v, w, /**/ d);
    zm = bbox_zhi(bbox);
    vec_ini(u, v, max(zm, w) + eps, /**/ e);
    tri2list_tris(q->list, u, v, &tris);
    m = 0;
    while ((t = *tris++) != -1) {
        he_tri_ijk(he, t, &i, &j, &k);
        vec_get(i, x, y, z, a);
        vec_get(j, x, y, z, b);
        vec_get(k, x, y, z, c);
        m += predicate_ray(d, e, a, b, c);
    }
    return m % 2;
}

int
surface_distance(T * q, /**/ real x0, real y0, real z0, real * p)
{
    int t, n, i, j, k;
    real a[3], b[3], c[3], r[3], d, m;
    He *he;
    const real *x, *y, *z;
    int status;

    n = he_nt(he);
    vec_ini(x0, y0, z0, r);
    m = 0;
    he = q->he;
    x = q->x;
    y = q->y;
    z = q->z;
    for (t = 0; t < n; t++) {
        status = he_tri_ijk(he, t, &i, &j, &k);
	if (status != CO_OK)
	    ERR(CO_NUM, "he_tri_ijk failed (t=%d, n=%d)", t, n);
        vec_get(i, x, y, z, a);
        vec_get(j, x, y, z, b);
        vec_get(k, x, y, z, c);
        d = tri_point_distance2(a, b, c, r);
        if (d > m)
            m = d;
    }
    d = sqrt(m);
    if (surface_inside(q, x0, y0, z0))
        *p = d;
    else
        *p = -d;
    return CO_OK;
}
