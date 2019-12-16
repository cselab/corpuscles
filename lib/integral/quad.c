#include <stdio.h>
#include <math.h>
#include "real.h"
#include "co/err.h"
#include "co/macro.h"
#include "co/memory.h"
#include "co/tri.h"
#include "co/integral/quad.h"

#define T IntegralQuad
#define FMT CO_REAL_OUT

static real U[] = {
    0.659027622374092,
    0.659027622374092,
    0.231933368553031,
    0.231933368553031,
    0.109039009072877,
    0.109039009072877,
};

static real V[] = {
    0.231933368553031,
    0.109039009072877,
    0.659027622374092,
    0.109039009072877,
    0.659027622374092,
    0.231933368553031,
};

static real W[] = {
    0.16666666666666666667,
    0.16666666666666666667,
    0.16666666666666666667,
    0.16666666666666666667,
    0.16666666666666666667,
    0.16666666666666666667,
};

struct T {

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
integral_quad_ini(T ** pq)
{
    T *q;

    MALLOC(1, &q);
    *pq = q;
    return CO_OK;
}

int
integral_quad_fin(T * q)
{
    FREE(q);
    return CO_OK;
}

int
integral_quad_apply(T * q, const real a[3], const real b[3],
                    const real c[3], real(*f) (real, real, real, void *),
                    void *param, /**/ real * pres)
{
    USED(q);
    struct Param p;
    real A, u, v, w, res;
    int i;

    p.f = f;
    p.param = param;
    p.a = a;
    p.b = b;
    p.c = c;
    A = tri_area(a, b, c);

    res = 0;
    for (i = 0; i < (int) (sizeof(W) / sizeof(W[0])); i++) {
        u = U[i];
        v = V[i];
        w = W[i];
        res += w * F(v, u, &p);
    }
    res *= A;
    *pres = res;
    return CO_OK;
}
