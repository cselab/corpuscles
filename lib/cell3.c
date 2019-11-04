#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>

#include "real.h"
#include "co/err.h"
#include "co/macro.h"
#include "co/memory.h"

#include "co/list/c.h"
#include "co/cell3.h"

#define T Cell3
#define FMT CO_REAL_OUT

enum {
    X, Y, Z
};
struct T {
    int ny, nz;
    real lo[3], hi[3], size;
    Clist *clist;
    int (*wrp)(T * q, real *, real *, real *);
    int (*brn)(T * q, real, real, real, real *, real *, real *);
};

#define SIDE(n, D) \
	do { \
		l = hi[D] - lo[D]; \
		n = l / size; \
		if (n * size < l) n++; \
	} while (0)
static int
ini(const real lo[3], const real hi[3], real size,
    int (*gen)(int, int, int, Clist **), T ** pq)
{
    T *q;
    int nx, ny, nz;
    real l;
    Clist *clist;

    MALLOC(1, &q);

    SIDE(nx, X);
    SIDE(ny, Y);
    SIDE(nz, Z);

    gen(nx, ny, nz, &clist);

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

#define WRP(d, D) \
	do { \
		if (*d > hi[D]) *d -= (hi[D] - lo[D]); \
		if (*d < lo[D]) *d += (hi[D] - lo[D]); \
	} while (0)
static
    int
wrp_ppp(T * q, real * x, real * y, real * z)
{
    real *lo, *hi;

    lo = q->lo;
    hi = q->hi;
    WRP(x, X);
    WRP(y, Y);
    WRP(z, Z);
    return CO_OK;
}

int
wrp_ppn(T * q, real * x, real * y, __UNUSED real * z)
{
    real *lo, *hi;

    lo = q->lo;
    hi = q->hi;
    WRP(x, X);
    WRP(y, Y);
    return CO_OK;
}

int
wrp_pnn(T * q, real * x, __UNUSED real * y, __UNUSED real * z)
{
    real *lo, *hi;

    lo = q->lo;
    hi = q->hi;
    WRP(x, X);
    return CO_OK;
}


#define BRN(a, b, l) \
	do { \
		if (fabs(*a - b) > fabs(*a + l - b)) *a += l; \
		else if (fabs(*a - b) > fabs(*a - l - b)) *a -= l; \
	} while (0)
static
    int
brn_ppp(T * q, real x, real y, real z, real * u, real * v, real * w)
{
    real lx, ly, lz;

    lx = q->hi[X] - q->lo[X];
    ly = q->hi[Y] - q->lo[Y];
    lz = q->hi[Z] - q->lo[Z];
    BRN(u, x, lx);
    BRN(v, y, ly);
    BRN(w, z, lz);
    return CO_OK;
}

static
    int
brn_ppn(T * q, real x, real y, __UNUSED real z, real * u, real * v,
        __UNUSED real * w)
{
    real lx, ly;

    lx = q->hi[X] - q->lo[X];
    ly = q->hi[Y] - q->lo[Y];
    BRN(u, x, lx);
    BRN(v, y, ly);
    return CO_OK;
}

static
    int
brn_pnn(T * q, real x, __UNUSED real y, __UNUSED real z, real * u,
        __UNUSED real * v, __UNUSED real * w)
{
    real lx;

    lx = q->hi[X] - q->lo[X];
    BRN(u, x, lx);
    return CO_OK;
}

int
cell3_ppp_ini(const real lo[3], const real hi[3], real size, T ** pq)
{
    int status;

    status = ini(lo, hi, size, clist_gen_ppp, pq);
    (*pq)->wrp = wrp_ppp;
    (*pq)->brn = brn_ppp;
    return status;
}

int
cell3_ppn_ini(const real lo[3], const real hi[3], real size, T ** pq)
{
    int status;

    status = ini(lo, hi, size, clist_gen_ppn, pq);
    (*pq)->wrp = wrp_ppn;
    (*pq)->brn = brn_ppn;
    return status;
}

int
cell3_pnn_ini(const real lo[3], const real hi[3], real size, T ** pq)
{
    int status;

    status = ini(lo, hi, size, clist_gen_pnn, pq);
    (*pq)->wrp = wrp_pnn;
    (*pq)->brn = brn_pnn;
    return status;
}

int
cell3_nnn_ini(const real lo[3], const real hi[3], real size, T ** pq)
{
    int status;

    status = ini(lo, hi, size, clist_gen_nnn, pq);
    (*pq)->wrp = NULL;
    (*pq)->brn = NULL;
    return status;
}

int
cell3_fin(T * q)
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

#define IDX  (i*ny*nz + j*nz + k)
int
cell3_push(T * q, int n, const real * x, const real * y, const real * z)
{
    int i, j, k, m;
    int ny, nz;

    clist_reset(q->clist);
    ny = q->ny;
    nz = q->nz;
    for (m = 0; m < n; m++) {
        map(q, x[m], y[m], z[m], &i, &j, &k);
        if (clist_push(q->clist, IDX, m) != CO_OK) {
            MSG("ijk: %d %d %d", i, j, k);
            MSG("nyz: %d %d", ny, nz);
            ERR(CO_INDEX, "fail to push particle: " FMT " " FMT " " FMT,
                x[m], y[m], z[m]);
        }
    }
    return CO_OK;
}

int
cell3_parts(T * q, real x, real y, real z, int **a)
{
    int i, j, k, ny, nz;

    ny = q->ny;
    nz = q->nz;
    map(q, x, y, z, &i, &j, &k);
    if (clist_parts(q->clist, IDX, a) != CO_OK) {
        MSG("ijk: %d %d %d", i, j, k);
        MSG("nyz: %d %d", ny, nz);
        ERR(CO_INDEX, "clist_parts failed" FMT " " FMT " " FMT, x, y, z);
    }
    return CO_OK;
}

int
cell3_len(T * q, real x, real y, real z)
{
    int i, j, k, ny, nz;

    ny = q->ny;
    nz = q->nz;
    map(q, x, y, z, &i, &j, &k);
    return clist_len(q->clist, IDX);
}

int
cell3_wrap(T * q, int n, real * x, real * y, real * z)
{
    int i;

    if (q->wrp == NULL)
        return CO_OK;
    for (i = 0; i < n; i++)
        q->wrp(q, &x[i], &y[i], &z[i]);
    return CO_OK;
}

int
cell3_bring(T * q, real x, real y, real z, real * u, real * v, real * w)
{
    if (q->brn == NULL)
        return CO_OK;
    return q->brn(q, x, y, z, u, v, w);
}
