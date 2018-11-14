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
#include "he/memory.h"
#include "he/laplace.h"

#define T Laplace

#define BEGIN_LOOP                               \
    nh = he_nh(he);                              \
    for (h = 0; h < nh; h++) {                   \
    he_ijk(he, h, &i, &j, &k);                   \
    vec_get(i, x, y, z, a);                      \
    vec_get(j, x, y, z, b);                      \
    vec_get(k, x, y, z, c);
#define END_LOOOP }

typedef struct Vec Vec;
struct Vec { real v[3]; };

struct T {
    int nv, nh;
    real *tb, *tc;
    Vec *eb, *ec;
    real *area, *lx, *ly, *lz;
};

int laplace_ini(He *he, /**/ T **pq) {
    int nv, nh;
    T *q;
    MALLOC(1, &q);

    nv = he_nv(he);
    nh = he_nh(he);

    MALLOC(nh, &q->tb);
    MALLOC(nh, &q->tc);
    MALLOC(nh, &q->eb);
    MALLOC(nh, &q->ec);
    MALLOC(nv, &q->area);
    MALLOC(nv, &q->lx);
    MALLOC(nv, &q->ly);
    MALLOC(nv, &q->lz);

    q->nv = nv;
    q->nh = nh;
    *pq = q;
    return HE_OK;
}

int laplace_fin(T *q) {
    FREE(q->tb); FREE(q->tc);
    FREE(q->eb); FREE(q->ec); FREE(q->area);
    FREE(q->lx); FREE(q->ly); FREE(q->lz);
    FREE(q);
    return HE_OK;
}

int laplace_apply(T *q, He *he, const real *x, const real *y, const real *z,
                  /**/ real **lx, real **ly, real **lz) {
    return HE_OK;
}
