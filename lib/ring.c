#include <stdio.h>

#include "real.h"

#include "co/err.h"
#include "co/memory.h"
#include "inc/def.h"
#include "co/ring.h"

#define T Ring
#define N (RANK_MAX + 1)

struct T {
    real alpha[N], beta[N], theta[N];
    real xyz[3*N], A[6*N];
};

int ring_ini(T **pq) {
    T *q;
    MALLOC(1, &q);
    *pq = q;
    return CO_OK;
}

int ring_fin(T *q) {
    FREE(q);
}
