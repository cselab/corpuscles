#include <stdio.h>
#include <math.h>

#include "he/memory.h"
#include "he/err.h"

#include "real.h"

#define T HeSum

typedef struct T T;
struct T { real c, sum; };

int he_sum_ini(/**/ T **pq) {
    T *q;
    MALLOC(1, &q);
    q->c = q->sum = 0;
    *pq = q;
    return HE_OK;
}

int he_sum_fin(T *q) { FREE(q); return HE_OK; }
int he_sum_add(T *q, real input) {
    real t;
    real y, c, sum;
    c = q->c; sum = q->sum;
    
    y = input - c;
    t = sum + y;
    c = (t - sum) - y;
    sum = t;

    q->c = c; q->sum = sum;
    return HE_OK;
}

real he_sum_get(const T *q) { return q->sum; }
int he_sum_compensation(const T *q) { return q->c; }

