#include <stdio.h>

#include "real.h"
#include "co/memory.h"
#include "co/err.h"
#include "co/he.h"
#include "co/vec.h"
#include "co/sum.h"
#include "co/f/dvolume.h"
#include "co/restore.h"

#define T Restore

struct T {
    int n;
    real volume;
    real *fx, *fy, *fz;
    HeFDvolume *dvolume;
};

int
restore_ini(real volume, He * he, T ** pq)
{
    T *q;
    int n;

    MALLOC(1, &q);

    n = he_nv(he);

    q->n = n;
    q->volume = volume;
    MALLOC(n, &q->fx);
    MALLOC(n, &q->fy);
    MALLOC(n, &q->fz);

    he_f_dvolume_ini(he, &q->dvolume);

    *pq = q;
    return CO_OK;
}

int
restore_fin(T * q)
{
    FREE(q->fx);
    FREE(q->fy);
    FREE(q->fz);
    he_f_dvolume_fin(q->dvolume);
    FREE(q);
    return CO_OK;
}

static real
sum_sq(int n, real * x, real * y, real * z)
{
    real v, w;
    int i;
    HeSum *sum;

    sum_ini(&sum);
    for (i = 0; i < n; i++) {
        w = x[i] * x[i] + y[i] * y[i] + z[i] * z[i];
        sum_add(sum, w);
    }
    v = sum_get(sum);
    sum_fin(sum);
    return v;
}

static int
zero(int n, real * a)
{
    int i;

    for (i = 0; i < n; i++)
        a[i] = 0;
    return CO_OK;
}

static int
add(int n, real step,
    const real * fx, const real * fy, const real * fz,
    real * x, real * y, real * z)
{
    int i;

    for (i = 0; i < n; i++) {
        x[i] += step * fx[i];
        y[i] += step * fy[i];
        z[i] += step * fz[i];
    }
    return CO_OK;
}

int
restore_volume(T * q, He * he, /**/ real * x, real * y, real * z)
{
    int n, status;
    real volume, volume0, delta, lambda, step;
    HeFDvolume *dvolume;
    real *fx, *fy, *fz;

    volume0 = q->volume;
    dvolume = q->dvolume;
    fx = q->fx;
    fy = q->fy;
    fz = q->fz;
    n = he_nv(he);
    zero(n, fx);
    zero(n, fy);
    zero(n, fz);
    status = he_f_dvolume_force(dvolume, he, x, y, z, /**/ fx, fy, fz);
    if (status != CO_OK)
        ERR(CO_NUM, "he_f_dvolume_force failed");
    volume = he_f_dvolume_energy(dvolume, he, x, y, z);
    lambda = sum_sq(n, fx, fy, fz);
    delta = volume - volume0;
    step = -delta / lambda;
    add(n, step, fx, fy, fz, x, y, z);

    return CO_OK;
}
