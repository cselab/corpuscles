#include <stdio.h>

#include "real.h"
#include "he/memory.h"
#include "he/err.h"
#include "he/he.h"
#include "he/vec.h"
#include "he/sum.h"

#include "he/restore.h"

#define T Restore

struct T {
    int n;
    real volume;
    real *fx, *fy, *fz;
};

static real sum(int n, real *a) { return he_sum_array(n, a); }

int restore_ini(real volume, He *he, T **pq) {
    T *q;
    int n;
    MALLOC(1, &q);

    n = he_nv(he);

    MALLOC(n, &q->fx); MALLOC(n, &q->fy); MALLOC(n, &q->fz);
    q->n = n;

    *pq = q;
    return HE_OK;
}

int restore_fin(T *q) {
    FREE(q->fx); FREE(q->fy); FREE(q->fz);
    FREE(q);
    return HE_OK;
}

int restore_volume(T *q, He *he, /**/
                    real *x, real *y, real *z) {
    return HE_OK;
}
