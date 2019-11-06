#include <stdio.h>
#include <math.h>
#include "real.h"
#include <alg/integration.h>
#include <alg/integration2.h>
#include "co/err.h"
#include "co/macro.h"
#include "co/memory.h"
#include "co/tri.h"
#include "co/integral/tri.h"

enum {
    TYPE = QAGS
};

#define T IntegralTri
#define FMT CO_REAL_OUT

struct T {
    AlgIntegration2 *integ;
};

struct Param {
    const real *a, *b, *c;
     real(*f) (real, real, real, void *);
    void *param;
};

static real
F(real u, real v, void *vp)
{
    enum { X, Y, Z };
    struct Param *p;
    real ax, ay, az, bx, by, bz, cx, cy, cz;
    real x, y, z;

    p = vp;
    ax = p->a[X];
    ay = p->a[Y];
    az = p->a[Z];

    bx = p->b[X];
    by = p->b[Y];
    bz = p->b[Z];

    cx = p->c[X];
    cy = p->c[Y];
    cz = p->c[Z];

    x = (cx - ax) * v + (bx - ax) * u + ax;
    y = (cy - ay) * v + (by - ay) * u + ay;
    z = (cz - az) * v + (bz - az) * u + az;

    return p->f(x, y, z, p->param);
}


int
integral_tri_ini(T ** pq)
{
    T *q;
    int status;

    MALLOC(1, &q);
    status = alg_integration2_ini(TYPE, &q->integ);
    if (status != CO_OK)
        ERR(CO_MEMORY, "alg_integration2_ini failed");

    *pq = q;
    return CO_OK;
}

int
integral_tri_fin(T * q)
{
    alg_integration2_fin(q->integ);
    FREE(q);
    return CO_OK;
}

static real
v0(real u, void *p)
{
    USED(u);
    USED(p);
    return 0;
}

static real
v1(real u, void *p)
{
    USED(p);
    return 1 - u;
}

int
integral_tri_apply(T * q, const real a[3], const real b[3],
                   const real c[3], real(*f) (real, real, real, void *),
                   void *param, /**/ real * pres)
{
    struct Param p;
    real A, u0, u1, res;
    int status;

    p.f = f;
    p.param = param;
    p.a = a;
    p.b = b;
    p.c = c;
    A = tri_area(a, b, c);
    u0 = 0;
    u1 = 1;
    status =
        alg_integration2_apply(q->integ, u0, u1, &v0, &v1, &F, &p, &res);
    if (status != CO_OK)
        ERR(CO_NUM, "alg_integration2_apply failed");
    res *= 2 * A;
    *pres = res;
    return CO_OK;
}
