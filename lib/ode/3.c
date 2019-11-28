#include <stdio.h>
#include "real.h"
#include <alg/ode.h>
#include "co/array.h"
#include "co/err.h"
#include "co/macro.h"
#include "co/memory.h"
#include "co/ode/3.h"
#include "co/skel.h"

#define T Ode3

struct T {
    int n;
    real *r, *x, *y, *z, *fx, *fy, *fz;
    Ode *ode;
    int (*fun)(real, const real *, const real *, const real *, real *,
               real *, real *, void *);
    void *p;
};

static int
F(__UNUSED real t, const real * r, real * f, void *q0)
{
    T *q;
    int n;
    real *x, *y, *z, *fx, *fy, *fz;
    void *p;

    q = q0;
    n = q->n;
    x = q->x;
    y = q->y;
    z = q->z;
    fx = q->fx;
    fy = q->fy;
    fz = q->fz;
    p = q->p;
    array_zero(n, fx);
    array_zero(n, fy);
    array_zero(n, fz);
    array_copy(n, r, x);
    array_copy(n, r + n, y);
    array_copy(n, r + 2 * n, z);
    if (q->fun(t, x, y, z, fx, fy, fz, p) != CO_OK)
        ERR(CO_NUM, "q->fun failed");
    array_copy(n, fx, f);
    array_copy(n, fy, f + n);
    array_copy(n, fz, f + 2 * n);
    return CO_OK;
}

int
ode3_ini(int type, int n, real dt,
         int (*fun)(real, const real *, const real *, const real *, real *,
                    real *, real *, void *), void *p, T ** pq)
{
    T *q;
    real *r, *x, *y, *z, *fx, *fy, *fz;
    Ode *ode;

    MALLOC(1, &q);
    MALLOC(3 * n, &r);
    MALLOC(n, &x);
    MALLOC(n, &y);
    MALLOC(n, &z);
    MALLOC(n, &fx);
    MALLOC(n, &fy);
    MALLOC(n, &fz);
    q->n = n;
    q->r = r;
    q->x = x;
    q->y = y;
    q->z = z;
    q->fx = fx;
    q->fy = fy;
    q->fz = fz;
    q->fun = fun;
    q->p = p;
    if (ode_ini(type, 3 * n, dt, F, q, &ode) != CO_OK)
        ERR(CO_MEMORY, "ode_ini failed");
    q->ode = ode;
    *pq = q;
    return CO_OK;
}

int
ode3_fin(T * q)
{
    FREE(q->r);
    FREE(q->x);
    FREE(q->y);
    FREE(q->z);
    FREE(q->fx);
    FREE(q->fy);
    FREE(q->fz);
    if (ode_fin(q->ode) != CO_OK)
        ERR(CO_MEMORY, "ode_fin failed");
    FREE(q);
    return CO_OK;
}

static int
apply(T * q, real * time, real t, real * x, real * y, real * z,
      int (*step)(Ode *, real *, real, real *))
{
    int n;
    real *r;
    Ode *ode;

    n = q->n;
    r = q->r;
    ode = q->ode;
    array_copy(n, x, r);
    array_copy(n, y, r + n);
    array_copy(n, z, r + 2 * n);
    if (step(ode, time, t, r) != CO_OK)
        ERR(CO_NUM, "ode_apply failed");
    array_copy(n, r, x);
    array_copy(n, r + n, y);
    array_copy(n, r + 2 * n, z);
    return CO_OK;
}

int
ode3_apply(T * q, real * time, real t, real * x, real * y, real * z)
{
    return apply(q, time, t, x, y, z, ode_apply);
}

int
ode3_apply_fixed(T * q, real * time, real t, real * x, real * y, real * z)
{
    return apply(q, time, t, x, y, z, ode_apply_fixed);
}
