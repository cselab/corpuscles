#include <stdio.h>
#include "real.h"
#include "co/array.h"
#include "co/bi.h"
#include "co/err.h"
#include "co/memory.h"
#include "co/subst.h"

#define FMT CO_REAL_OUT
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

static int
apply(T * q, He * he, BI * bi,
      const real * x, const real * y, const real * z,
      const real * ux, const real * uy, const real * uz,
      real * ix, real * iy, real * iz, real * vx0, real * vy0, real * vz0)
{
    int status, n, niter, iiter;
    real *wx, *wy, *wz, alpha, tol, diff, norm;
    int i;

    alpha = q->alpha;
    tol = q->tol;
    n = q->n;
    niter = q->niter;
    wx = q->wx;
    wy = q->wy;
    wz = q->wz;
    for (iiter = 0; iiter < niter; iiter++) {
        array_copy3(n, ux, uy, uz, wx, wy, wz);
        if (bi_double(bi, he, alpha, x, y, z, ix, iy, iz, wx, wy, wz) != CO_OK)
            goto fail;
	norm = array_inf3(n, wx, wy, wz);
        diff = array_diff_inf3(n, wx, wy, wz, ix, iy, iz);
        array_copy3(n, wx, wy, wz, ix, iy, iz);
        if (diff <= tol * norm)
            goto tol;
    }
    q->iiter = iiter;
    q->status = SUBST_ITER;
    array_copy3(n, ix, iy, iz, vx0, vy0, vz0);
    return CO_OK;
  tol:
    q->iiter = iiter;
    q->status = SUBST_TOL;
    array_copy3(n, ix, iy, iz, vx0, vy0, vz0);
    return CO_OK;
  fail:
    q->iiter = iiter;
    q->status = SUBST_FAIL;
    ERR(CO_NUM, "subst_apply failed (n=%d, iiter=%d", n, iiter);
}

int
subst_apply(T * q, He * he, BI * bi,
            const real * x, const real * y, const real * z,
            const real * ux, const real * uy, const real * uz,
            real * vx0, real * vy0, real * vz0)
{
    return apply(q, he, bi, x, y, z, ux, uy, uz, q->vx, q->vy, q->vz,
                 vx0, vy0, vz0);
}

int
subst_apply_initial(T * q, He * he, BI * bi,
                    const real * x, const real * y, const real * z,
                    const real * ux, const real * uy, const real * uz,
                    const real * ix, const real * iy, const real * iz,
                    real * vx0, real * vy0, real * vz0)
{
    array_copy3(q->n, ix, iy, iz, q->vx, q->vy, q->vz);
    return apply(q, he, bi, x, y, z, ux, uy, uz, q->vx, q->vy, q->vz,
                 vx0, vy0, vz0);
}


int
subst_status(T * q)
{
    return q->status;
}

int
subst_niter(T * q)
{
    return q->iiter;
}
