#include <stdio.h>
#include "real.h"
#include "alg/ode.h"
#include "co/array.h"
#include "co/err.h"
#include "co/force2.h"
#include "co/macro.h"
#include "co/memory.h"
#include "co/ode/2.h"
#include "co/skel.h"

#define T Ode2

struct T {
    int n;
    real *r, *x, *y, *fx, *fy;
    Ode *ode;
    int (*fun)(real, const real *, const real *, real *, real *, void *);
    void *p;
};

static int
F(__UNUSED real t, const real * r, real * f, void *q0)
{
    T *q;
    int n;
    real *x, *y, *fx, *fy;
    void *p;

    q = q0;
    n = q->n;
    x = q->x;
    y = q->y;
    fx = q->fx;
    fy = q->fy;
    p = q->p;
    array_zero(n, fx);
    array_zero(n, fy);
    array_copy(n, r, x);
    array_copy(n, r + n, y);
    if (q->fun(t, x, y, fx, fy, p) != CO_OK)
        ERR(CO_NUM, "q->fun failed");
    array_copy(n, fx, f);
    array_copy(n, fy, f + n);
    return CO_OK;
}

int
ode2_ini(int type, int n, real dt,
         int (*fun)(real, const real *, const real *, real *, real *,
                    void *), void *p, T ** pq)
{
    T *q;
    real *r, *x, *y, *fx, *fy;
    Ode *ode;

    MALLOC(1, &q);
    MALLOC(2 * n, &r);
    MALLOC(n, &x);
    MALLOC(n, &y);
    MALLOC(n, &fx);
    MALLOC(n, &fy);
    q->n = n;
    q->r = r;
    q->x = x;
    q->y = y;
    q->fx = fx;
    q->fy = fy;
    q->fun = fun;
    q->p = p;
    if (ode_ini(type, 2 * n, dt, F, q, &ode) != CO_OK)
        ERR(CO_MEMORY, "ode_ini failed");
    q->ode = ode;
    *pq = q;
    return CO_OK;
}

int
ode2_fin(T * q)
{
    FREE(q->r);
    FREE(q->x);
    FREE(q->y);
    FREE(q->fx);
    FREE(q->fy);
    if (ode_fin(q->ode) != CO_OK)
        ERR(CO_MEMORY, "ode_fin failed");
    FREE(q);
    return CO_OK;
}

int
ode2_apply(T * q, real * time, real t, real * x, real * y)
{
    int n;
    real *r;
    Ode *ode;

    n = q->n;
    r = q->r;
    ode = q->ode;
    array_copy(n, x, r);
    array_copy(n, y, r + n);
    if (ode_apply(ode, time, t, r) != CO_OK)
        ERR(CO_NUM, "ode_apply failed");
    array_copy(n, r, x);
    array_copy(n, r + n, y);
    return CO_OK;
}
