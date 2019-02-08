#include <stdio.h>

#include "real.h"

#include "he/he.h"
#include "he/invariants.h"
#include "he/err.h"
#include "he/memory.h"
#include "he/vec.h"
#include "he/tri.h"
#include "he/y.h"

#define T Invariants

#define BEGIN                                                   \
for (t = 0; t < nt; t++) {                                      \
    he_tri_ijk(he, t, /**/ &i, &j, &k);                         \
    get3(q->x, q->y, q->z, i, j, k, /**/ a0, b0, c0);           \
    get3(x, y, z, i, j, /**/ k, a, b, c);
#define END }

struct T {
    He *he;
    real *x, *y, *z;
    real *al, *be;
};

int invariants_ini(FILE *f, /**/ T **pq) {
    T *q;
    int n;
    real *al, *be;
    He *he;

    MALLOC(1, &q);

    y_inif(f, &he, &q->x, &q->y, &q->z);
    n = he_nv(he);
    MALLOC(n, &al);
    MALLOC(n, &be);

    q->he = he;
    q->al = al; q->be = be;

    return HE_OK;
}

int invariants_fin(T *q) {
    y_fin(q->he, q->x, q->y, q->z);
    FREE(q->al); FREE(q->be);

    FREE(q);
    return HE_OK;
}

static int get3(const real *x, const real *y, const real *z,
                int i, int j, int k,  /**/
                real a[3], real b[3], real c[3]) {
    vec_get(i, x, y, z, /**/ a);
    vec_get(j, x, y, z, /**/ b);
    vec_get(k, x, y, z, /**/ c);
    return HE_OK;
}

int invariants_al(T *q, const real *x, const real *y, const real *z, real **a) {


    return HE_OK;
}

int invariants_be(T *q, const real *x, const real *y, const real *z, real **a) {
    return HE_OK;
}
