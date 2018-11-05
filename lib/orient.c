#include <stdio.h>

#include "real.h"
#include "he/memory.h"
#include "he/err.h"
#include "he/he.h"
#include "he/vec.h"
#include "he/sum.h"

#include "he/orient.h"

#define T Orient

struct T {
    int n;
    real *x, *y, *z;
};

static real sum(int n, real *a) { return he_sum_array(n, a); }

int orient_ini(He *he, T **pq) {
    T *q;
    int n;
    MALLOC(1, &q);

    n = he_nv(he);

    q->n = n;
    MALLOC(n, &q->x); MALLOC(n, &q->y); MALLOC(n, &q->z);

    *pq = q;
    return HE_OK;
}

int orient_fin(T *q) {
    FREE(q->x); FREE(q->y); FREE(q->z);
    FREE(q);
    return HE_OK;
}

int orient_apply(T *q, real *x, real *y, real *z) {
    return HE_OK;
}
