#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>

#include "real.h"
#include "co/err.h"
#include "co/he.h"
#include "co/macro.h"
#include "co/memory.h"
#include "co/tri.h"
#include "co/vec.h"

#include "co/list/c.h"
#include "co/list/tri2.h"

#define T Tri2List
#define FMT CO_REAL_OUT

enum {
    X, Y
};

struct T {
    int ny;
    real lo[2], hi[2], size;
    Clist *clist;
};

#define SIDE(n, D) \
	do { \
		l = hi[D] - lo[D]; \
		n = l / size; \
		if (n * size < l) n++; \
	} while (0)

int
tri2list_ini(const real lo[2], const real hi[2], real size, T ** pq)
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
tri2list_fin(T * q)
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
cm(He * he, const real * x, const real * y, int m, /**/ real * u, real * v)
{
    int i, j, k;

    he_tri_ijk(he, m, &i, &j, &k);
    *u = (x[i] + x[j] + x[k]) / 3;
    *v = (y[i] + y[j] + y[k]) / 3;
    return CO_OK;
}

#define IDX  (i*ny + j)
int
tri2list_push(T * q, He * he, const real * x, const real * y)
{
    int i, j, m;
    int ny, n;
    real u, v;

    clist_reset(q->clist);
    ny = q->ny;
    n = he_nt(he);

    for (m = 0; m < n; m++) {
        cm(he, x, y, m, &u, &v);
        map(q, u, v, &i, &j);
        if (clist_push(q->clist, IDX, m) != CO_OK) {
            MSG("ijk: %d %d %d", i, j);
            MSG("ny: %d", ny);
            ERR(CO_INDEX, "fail to push  traingle " FMT " " FMT, u, v);
        }
    }
    return CO_OK;
}

int
tri2list_tris(T * q, real x, real y, int **ptris)
{
    int i, j, ny, *tris;

    ny = q->ny;
    map(q, x, y, &i, &j);
    if (clist_parts(q->clist, IDX, &tris) != CO_OK) {
        MSG("ijk: %d %d", i, j);
        MSG("nyz: %d", ny);
        ERR(CO_INDEX, "clist_parts failed " FMT " " FMT, x, y);
    }
    *ptris = tris;
    return CO_OK;
}
