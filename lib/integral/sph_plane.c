#include <stdio.h>
#include <math.h>
#include "real.h"
#include "co/err.h"
#include "co/macro.h"
#include "co/memory.h"
#include "co/integral/sph_plane.h"
#include "alg/integration.h"

enum {
    TYPE = GAUSS31
};

#define T SphPlane
#define FMT CO_REAL_OUT
#define PI (3.141592653589793)

struct T {
    AlgIntegration *ie, *ih, *ig;
    real R, d;
    real p0, p, t;
    void *param;
     real(*E) (real, real, real, void *);
};

int
sph_plane_ini(real R, T ** pq)
{
    T *q;

    MALLOC(1, &q);
    q->R = R;
    alg_integration_ini(TYPE, &q->ie);
    alg_integration_ini(TYPE, &q->ih);
    alg_integration_ini(TYPE, &q->ig);
    *pq = q;
    return CO_OK;
}

int
sph_plane_fin(T * q)
{
    alg_integration_fin(q->ie);
    alg_integration_fin(q->ih);
    alg_integration_fin(q->ig);
    FREE(q);
    return CO_OK;
}

static real
f(real r, void *v)
{
    void *param;
    real p, t, x, y, z;
    T *q;

    q = v;
    p = q->p;
    t = q->t;
    param = q->param;
    x = r * cos(t) * sin(p);
    y = r * sin(t) * sin(p);
    z = r * cos(p);
    return r * r * sin(p) * q->E(x, y, z, param);
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
g(real p, void *v)
{
    real a, b, d, R, cp, res;
    T *q;

    q = v;
    q->p = p;
    d = q->d;
    R = q->R;
    cp = cos(p);
    a = small(cp) ? 0 : -d / cp;
    b = R;
    alg_integration_apply(q->ig, a, b, f, v, &res);
    return res;
}

static real
h(real tt, void *v)
{
    real a, b, res;
    T *q;

    q = v;
    q->t = tt;
    a = q->p0;
    b = PI;
    alg_integration_apply(q->ih, a, b, g, v, &res);
    return res;
}

int
sph_plane_apply(T * q, real d, real(*E) (real, real, real, void *),
                void *param, /**/ real * res)
{
    int status;
    real R, a, b;

    R = q->R;
    if (d < 0)
        ERR(CO_NUM, "d=" FMT "< 0", d);
    if (d > R)
        ERR(CO_NUM, "d=" FMT " > R=" FMT, d, R);
    q->E = E;
    q->param = param;
    q->d = d;
    q->p0 = acos(-d / R);
    a = 0;
    b = 2 * PI;
    status = alg_integration_apply(q->ie, a, b, h, q, res);
    if (status != CO_OK)
        ERR(CO_NUM, "alg_integration failed");

    return CO_OK;
}

static real
one(__UNUSED real x, __UNUSED real y, __UNUSED real z,
    __UNUSED void *param)
{
    return 1;
}

int
sph_plane_volume(T * q, real d, /**/ real * result)
{

    return sph_plane_apply(q, d, one, NULL, result);
}
