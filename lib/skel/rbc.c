#include <stdio.h>
#include <tgmath.h>

#include "real.h"
#include "co/arc.h"
#include "co/err.h"
#include "co/macro.h"
#include "co/memory.h"
#include "co/skel.h"

#define T Skel
#define PI (3.141592653589793)

static const real A = 0.54353, B = 0.121435, C = -0.561365;
static real
F(real q)
{
    return A * q + B * q * q * q + C * q * q * q * q * q;
}

static real
dF(real q)
{
    return A + 3 * B * q * q + 5 * C * q * q * q * q;
}

static real
x(real t, __UNUSED void *p)
{
    return sin(t);
}

static real
y(real t, __UNUSED void *p)
{
    return F(cos(t));
}

static real
dx(real t, __UNUSED void *p)
{
    return cos(t);
}

static real
dy(real t, __UNUSED void *p)
{
    return -dF(cos(t)) * sin(t);
}

int
skel_rbc_ini(int n, /**/ real * plength, real ** pu, real ** pv, T ** pq)
{
    Arc *arc;
    real a, b, t, length, *points, *u, *v;
    int i;

    if (n < 0)
        ERR(CO_INDEX, "n=%d < 0", n);

    MALLOC(n, &u);
    MALLOC(n, &v);
    a = 0;
    b = 2 * PI;
    arc_xy_ini(n, a, b, dx, dy, NULL, &arc);
    arc_points(arc, &points);
    arc_length(arc, &length);
    for (i = 0; i < n; i++) {
        t = points[i];
        u[i] = x(t, NULL);
        v[i] = y(t, NULL);
    }
    skel_close_ini(n, pq);
    *pu = u;
    *pv = v;
    *plength = length;
    arc_fin(arc);
    return CO_OK;
}
