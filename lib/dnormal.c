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
#include "he/dnormal.h"

#define T Dnormal

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
    real *ang;
    Ten *Dn, *F;
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
    MALLOC(nv, &q->Dn);
    MALLOC(nv, &q->m);
    MALLOC(nv, &q->F);

    q->nv = nv;
    q->nh = nh;
    *pq = q;
    return HE_OK;
}

int dnormal_fin(T *q) {
    FREE(q->u); FREE(q->ang); FREE(q->n);
    FREE(q->Dn); FREE(q->m); FREE(q->F);
    FREE(q);
    return HE_OK;
}

static int QplusAbc(Ten *A, Vec b, Vec c, /**/ Ten *Q) {
    real x[3];
    Ten Y;
    ten_vec(A, b.v, /**/ x);
    ten_dyadic(x, c.v, /**/ &Y);
    ten_add(&Y, Q);
    return HE_OK;
}

int dnormal_apply(T *q, He *he, const real *x, const real *y, const real *z,
                  /**/ Ten **pf) {
    int nh, nv, h, i, j, k;
    real a[3], b[3], c[3];
    real da[3], db[3], dc[3];
    real *ang;
    Ten *Dn, *F;
    Vec *u, *m, *n;
    Ten Da, Db, Dc;

    u = q->u;
    ang = q->ang;
    m = q->m;
    F = q->F;
    Dn = q->Dn;
    n = q->n;

    nv = he_nv(he);
    for (i = 0; i < nv; i++) {
        vec_zero(m[i].v);
        ten_zero(&F[i]);
    }

    BEGIN_LOOP {
        tri_normal(a, b, c, /**/ u[h].v);
        ang[h] = tri_angle(c, a, b);
        vec_axpy(ang[h], u[h].v, /*io*/ m[i].v);
    } END_LOOOP;

    for (i = 0; i < nv; i++) {
        vec_norm(m[i].v, n[i].v);
        dvec_norm(m[i].v, &Dn[i]);
    }

    BEGIN_LOOP {
        dtri_normal(a, b, c, /**/ &Da, &Db, &Dc);
        ten_mult_left(&Dn[i], &Da);
        ten_mult_left(&Dn[i], &Db);
        ten_mult_left(&Dn[i], &Dc);

        ten_axpy(ang[h], &Da, &F[i]);
        ten_axpy(ang[h], &Db, &F[j]);
        ten_axpy(ang[h], &Dc, &F[k]);
    } END_LOOOP;

    BEGIN_LOOP {
        dtri_angle(c, a, b, dc, da, db);

    } END_LOOOP;

    return HE_OK;
}
