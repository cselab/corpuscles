#include <stdio.h>

#include "real.h"
#include "he/memory.h"
#include "he/err.h"
#include "he/he.h"

#include "he/f/area.h"

#define T HeFArea

struct T {
    int n;
    real *a, *da;

    real a0, k;
};

int he_f_area_ini(real a0, real k, He *he, T **pq) {
    T *q;
    int n;
    MALLOC(1, &q);
    n = he_nt(he);

    MALLOC(n, &q->da);
    MALLOC(n, &q->a);

    q->n = n;
    q->a0 = a0;
    q->k = k;
    
    *pq = q;
    return HE_OK;
}

int he_f_area_fin(T *q) {
    FREE(q->a); FREE(q->da); FREE(q);
    return HE_OK;
}

int he_f_area_a(T *q, /**/ real  **pa) {
    *pa = q->a;
    return HE_OK;
}

int he_f_area_da(T *q, /**/ real  **pa) {
    *pa = q->da;
    return HE_OK;
}
