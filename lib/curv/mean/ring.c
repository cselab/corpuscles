#include <tgmath.h>
#include <stdio.h>

#include "real.h"
#include "co/array.h"
#include "co/curv/mean/ring.h"
#include "co/dih.h"
#include "co/edg.h"
#include "co/err.h"
#include "co/he.h"
#include "co/memory.h"
#include "co/tri.h"
#include "co/ring.h"
#include "co/vec.h"

#define T CurvMeanRing

struct T {
    int nv;
    real *area;
    Ring *ring;
};

int
curv_mean_ring_ini(He * he, T **pq)
{
    T *q;
    int status;
    int nv;

    nv = he_nv(he);
    MALLOC(1, &q);
    MALLOC(nv, &q->area);
    ring_ini(&q->ring);
    q->nv = nv;
    *pq = q;
    return CO_OK;
}

int
curv_mean_ring_fin(T * q)
{
    FREE(q->area);
    ring_fin(q->ring);
    FREE(q);
    return CO_OK;
}

int
curv_mean_ring_apply(T * q, He * he, const real * x, const real * y, const real * z,
	/**/ real * ans)
{
    real a[3], b[3], c[3], d[3], A, *area;
    int i, j, k, l, t, nt, ne, nv;
    int e;
    real len, theta, coeff;
    
    nv = q->nv;
    if (nv != he_nv(he))
	ERR(CO_NUM, "nv=%d != he_nv(he)=%d", nv, he_nv(he));

    int status, n, *rring;
    real *C, *xyz;
    for (i = 0; i < nv; i++) {
	status = he_ring(he, i, &n, &rring);
	ring_xyz(q->ring, i, rring, x, y, z, &xyz);
	ring_C(q->ring, i, rring, x, y, z, &C);
    }
    
    return CO_OK;
}

int
curv_mean_ring_area(T * q, /**/ real ** ans)
{
    *ans = q->area;
    return CO_OK;
}
	
