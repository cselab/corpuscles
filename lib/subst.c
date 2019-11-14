#include <stdio.h>
#include "real.h"
#include "co/array.h"
#include "co/bi.h"
#include "co/err.h"
#include "co/memory.h"
#include "co/subst.h"

#define T Subst
struct T {
    int n, niter;
    real alpha, tol, *wx, *wy, *wz, *vx, *vy, *vz;
    int status, iiter;
};

int
subst_ini(int n, real alpha, real tol, int niter, T ** pq)
{
    T *q;

    MALLOC(1, &q);
    q->n = n;
    q->alpha = alpha;
    q->tol = tol;
    q->niter = niter;
    MALLOC3(n, &q->wx, &q->wy, &q->wz);
    CALLOC3(n, &q->vx, &q->vy, &q->vz);
    *pq = q;
    return CO_OK;
}

int
subst_fin(T * q)
{
    FREE3(q->wx, q->wy, q->wz);
    FREE3(q->vx, q->vy, q->vz);
    FREE(q);
    return CO_OK;
}

int
subst_apply(T * q, He * he, BI * bi,
            const real * x, const real * y, const real * z,
            const real * ux, const real * uy, const real * uz,
            real * vx0, real * vy0, real * vz0)
{
    int status, n, niter, iiter;
    real *wx, *wy, *wz, *vx, *vy, *vz, alpha, tol, diff, norm;

    alpha = q->alpha;
    tol = q->tol;
    n = q->n;
    niter = q->niter;
    wx = q->wx;
    wy = q->wy;
    wz = q->wz;
    vx = q->vx;
    vy = q->vy;
    vz = q->vz;
    for (iiter = 0; iiter < niter; iiter++) {
        array_copy3(n, ux, uy, uz, wx, wy, wz);
        status = bi_double(bi, he, alpha, x, y, z, vx, vy, vz, wx, wy, wz);
        if (status != CO_OK)
            goto fail;
        norm = array_msq_3d(n, wx, wy, wz);
        diff = array_l2_3d(n, wx, vx, wy, vy, wz, vz);
        array_copy3(n, wx, wy, wz, vx, vy, vz);
        if (diff <= tol * norm)
            goto tol;
    }
    q->iiter = iiter;
    q->status = SUBST_ITER;
    array_copy3(n, vx, vy, vz, vx0, vy0, vz0);
    return CO_OK;
  tol:
    q->iiter = iiter;
    q->status = SUBST_TOL;
    array_copy3(n, vx, vy, vz, vx0, vy0, vz0);
    return CO_OK;
  fail:
    q->iiter = iiter;
    q->status = SUBST_FAIL;
    ERR(CO_NUM, "subst_apply failed (n=%d, iiter=%d", n, iiter);
}

int
subst_status(T * q)
{
    return q->status;
}

int
subst_niter(T * q)
{
    return q->iiter + 1;
}
