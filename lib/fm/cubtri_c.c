#include <stdlib.h>
#include <stdio.h>
#include <real.h>
#include <co/err.h>
#include <co/memory.h>
#include <co/tri.h>

#include "cubtri.h"
#define T Cubtri
#define NW (5000)
static double eps = 1e-4;
static int mcalls = 1000;

struct T {

};

struct Function
{
  real (*function)(real, real, void *vp);
  void *params;
};

double
fun(void *p, double x, double y)
{
    struct Function *function;
    function = p;
    return function->function(x, y, function->params);
}

void
cubtri2(void*, double*, double, int, double*, double*, int*, double*, int, int*);

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
cubtri_ini(T ** pq)
{
    T *q;
    int status;

    MALLOC(1, &q);
    *pq = q;
    return CO_OK;
}

int
cubtri_fin(T * q)
{
    FREE(q);
    return CO_OK;
}

int
cubtri_apply(T * q, const real a[3], const real b[3],
		    const real c[3], real(*f) (real, real, real, void *),
		    void *param, /**/ real * pans)
{
    enum {X, Y};
    struct Param p;
    double w[NW], TT[2*3], err, ans;
    int i, status, ier, ncalls;
    real A;
    double a0[] = {0, 0};
    double b0[] = {1, 0};
    double c0[] = {0, 1};
    struct Function function;

    p.f = f;
    p.param = param;
    p.a = a;
    p.b = b;
    p.c = c;
    function.function = F;
    function.params = &p;
    i = 0;
    TT[i++] = a0[X];
    TT[i++] = a0[Y];
    TT[i++] = b0[X];
    TT[i++] = b0[Y];
    TT[i++] = c0[X];
    TT[i++] = c0[Y];

    ier = 0;
    cubtri2(&function, TT, eps, mcalls, &ans, &err, &ncalls, w, NW, &ier);
    if (ier != 0)
      ERR(CO_NUM, "cubtri2 failed");
    A = tri_area(a, b, c);
    *pans = A * ans / 2;
    return CO_OK;
}
