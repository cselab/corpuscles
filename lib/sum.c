#include <stdio.h>
#include <math.h>

#include "co/memory.h"
#include "co/err.h"

#include "real.h"

#define T HeSum

typedef struct T T;
struct T {
    real c, sum;
};

int
sum_ini( /**/ T ** pq)
{
    T *q;

    MALLOC(1, &q);
    q->c = q->sum = 0;
    *pq = q;
    return CO_OK;
}

int
sum_fin(T * q)
{
    FREE(q);
    return CO_OK;
}

int
sum_add(T * q, real input)
{
    real t;
    real y, c, sum;

    c = q->c;
    sum = q->sum;

    y = input - c;
    t = sum + y;
    c = (t - sum) - y;
    sum = t;

    q->c = c;
    q->sum = sum;
    return CO_OK;
}

real
sum_get(const T * q)
{
    return q->sum;
}

int
sum_compensation(const T * q)
{
    return q->c;
}

real
sum_array(int n, const real a[])
{
    int i;
    real v;
    T *sum;

    sum_ini(&sum);
    for (i = 0; i < n; i++)
        sum_add(sum, a[i]);
    v = sum_get(sum);
    sum_fin(sum);
    return v;
}
