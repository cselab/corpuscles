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
curv_mean_ring_ini(He * he, T ** pq)
{
    T *q;
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
curv_mean_ring_apply(T * q, He * he, const real * x, const real * y,
                     const real * z, /**/ real * ans)
{
    int nv, i;
    int n, *rring;
    real *C, *xyz;

    nv = q->nv;
    if (nv != he_nv(he))
        ERR(CO_NUM, "nv=%d != he_nv(he)=%d", nv, he_nv(he));
    for (i = 0; i < nv; i++) {
        he_ring(he, i, &n, &rring);
        ring_xyz(q->ring, i, rring, x, y, z, &xyz);
        ring_C(q->ring, i, rring, x, y, z, &C);
        ans[i] = ring_H(n, xyz, C);
    }

    return CO_OK;
}
