#include <math.h>
#include <stdio.h>

#include "real.h"
#include "he/err.h"
#include "he/he.h"
#include "he/vec.h"
#include "he/tri.h"
#include "he/ten.h"
#include "he/memory.h"
#include "he/dnormal.h"

#define T Dnormal

#define BEGIN_LOOP_HE                            \
    nh = he_nh(he);                              \
    for (h = 0; h < nh; h++) {                   \
    he_ijk(he, h, &i, &j, &k);                   \
    vec_get(i, x, y, z, a);                      \
    vec_get(j, x, y, z, b);                      \
    vec_get(k, x, y, z, c);
#define END_LOOOP_HE }

typedef struct Vec Vec;
struct Vec { real v[3]; };

struct T {
    int nv, nh;
    real *ang;
    Ten *dn, *f;
    Vec *u, *m, *n;
};

int dnormal_ini(He *he, /**/ T **pq) {
    int nv, nh;
    T *q;
    MALLOC(1, &q);

    nv = he_nv(he);
    nh = he_nh(he);

    MALLOC(nh, &q->u);
    MALLOC(nh, &q->ang);

    MALLOC(nv, &q->n);
    MALLOC(nv, &q->dn);
    MALLOC(nv, &q->m);
    MALLOC(nv, &q->f);

    q->nv = nv;
    q->nh = nh;
    *pq = q;
    return HE_OK;
}

int dnormal_fin(T *q) {
    FREE(q->u); FREE(q->ang); FREE(q->n);
    FREE(q->dn); FREE(q->m); FREE(q->f);
    FREE(q);
    return HE_OK;
}

int dnormal_apply(T *q, He *he, const real *x, const real *y, const real *z,
                  /**/ Ten **pf) {
    int nh, h, i, j, k;
    real a[3], b[3], c[3];

    BEGIN_LOOP_HE {

    } END_LOOOP_HE

    return HE_OK;
}
