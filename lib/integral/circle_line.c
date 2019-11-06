#include <stdio.h>
#include <math.h>
#include "real.h"
#include "co/err.h"
#include "co/macro.h"
#include "co/memory.h"
#include "co/integral/circle_line.h"
#include <alg/integration.h>

enum {
    TYPE = GAUSS31
};

#define T CircleLine
#define FMT CO_REAL_OUT
#define PI (3.141592653589793)

struct T {
    AlgIntegration *ie, *ih;
    real R, d, t;
    void *param;
     real(*E) (real, real, void *);
};

int
circle_line_ini(real R, T ** pq)
{
    T *q;

    MALLOC(1, &q);
    q->R = R;
    alg_integration_ini(TYPE, &q->ie);
    alg_integration_ini(TYPE, &q->ih);
    *pq = q;
    return CO_OK;
}

int
circle_line_fin(T * q)
{
    alg_integration_fin(q->ie);
    alg_integration_fin(q->ih);
    FREE(q);
    return CO_OK;
}

static real
f(real r, void *v)
{
    void *param;
    real t, x, y;
    T *q;

    q = v;
    t = q->t;
    param = q->param;
    x = r * cos(t);
    y = r * sin(t);
    return r * q->E(x, y, param);
}

static int
small(real s)
{
    real eps = 1e-6;

    if (s > eps)
        return 0;
    else if (s < -eps)
        return 0;
    else
        return 1;
}

static real
h(real t, void *v)
{
    real a, b, res, d, R;
    T *q;

    q = v;
    R = q->R;
    d = q->d;
    a = small(t) ? 0 : -d / sin(t);
    b = R;
    q->t = t;
    alg_integration_apply(q->ih, a, b, f, v, &res);
    return res;
}

int
circle_line_apply(T * q, real d, real(*E) (real, real, void *),
                  void *param, /**/ real * res)
{
    int status;
    real R, as, a, b;

    R = q->R;
    if (d < 0)
        ERR(CO_NUM, "d=" FMT "< 0", d);
    if (d > R)
        ERR(CO_NUM, "d=" FMT " > R=" FMT, d, R);
    q->E = E;
    q->param = param;
    q->d = d;
    as = asin(d / R);
    a = PI + as;
    b = 2 * PI - as;
    status = alg_integration_apply(q->ie, a, b, h, q, res);
    if (status != CO_OK)
        ERR(CO_NUM, "alg_integration failed");

    return CO_OK;
}

static real
one(__UNUSED real x, __UNUSED real y, __UNUSED void *param)
{
    return 1;
}

int
circle_line_volume(T * q, real d, /**/ real * result)
{
    return circle_line_apply(q, d, one, NULL, result);
}
