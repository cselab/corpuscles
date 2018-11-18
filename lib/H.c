#include <math.h>
#include <stdio.h>

#include "real.h"
#include "he/err.h"
#include "he/he.h"
#include "he/vec.h"
#include "he/dvec.h"
#include "he/tri.h"
#include "he/dtri.h"
#include "he/ten.h"
#include "he/edg.h"
#include "he/memory.h"
#include "he/laplace.h"
#include "he/H.h"

#define T H


struct T {
    int nv;
    Laplace *laplace;
    real *hh;
};

int H_ini(He *he, /**/ T **pq) {
    T *q;
    int status;
    int nv;
    MALLOC(1, &q);
    nv = he_nv(he);
    status = laplace_ini(he, &q->laplace);
    if (status != HE_OK)
        ERR(HE_NUM, "laplace_ini failed");
    MALLOC(nv, &q->hh);
    q->nv = nv;
    *pq = q;
    return status;
}

int H_fin(T *q) {
    laplace_fin(q->laplace);
    FREE(q->hh);
    FREE(q);
    return HE_OK;
}

int H_apply(T *q, He *he, const real *x, const real *y, const real *z,
                  /**/ real **pH, real **parea) {
    real *lx, *ly, *lz, *area;
    real *nx, *ny, *nz;

    laplace_apply(q->laplace, he, x, y, z, &lx, &ly, &lz, &area);
    normal_mwa(he, x, y, z, &nx, &ny, &nz);

    *parea = area;
    return HE_OK;
}
