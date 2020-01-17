#include <math.h>
#include <stdio.h>
#ifdef _OPENMP
#include <omp.h>
#endif
#include "real.h"
#include "co/err.h"
#include "co/macro.h"
#include "co/matrix.h"
#include "co/memory.h"
#include "co/oseen/2.h"
#include "co/skel.h"
#include "co/vec2.h"
#include "co/sum.h"

#define T Oseen2
static const real pi = 3.141592653589793115997964;
struct T {
    real e;
};

static int
oseen(__UNUSED real e, const real a[2], const real b[2], real * xx,
      real * xy, real * yy)
{
    enum {
        X, Y
    };
    real d[2], r, r2, l;

    vec2_minus(a, b, d);
    r = vec2_abs(d);
    if (r == 0)
        ERR(CO_NUM, "r == 0");
    r2 = r * r;
    l = log(r);
    *xx = -l + d[X] * d[X] / r2;
    *xy = d[X] * d[Y] / r2;
    *yy = -l + d[Y] * d[Y] / r2;
    return CO_OK;
}

static int
oseen0(real e, real * xx, real * xy, real * yy)
{
    real l;

    l = log(2 * e) - 3 / 2;
    *xx = -l;
    *xy = 0;
    *yy = -l;
    return CO_OK;
}

int
oseen2_ini(real e, T ** pq)
{
    T *q;

    MALLOC(1, &q);
    q->e = e;
    *pq = q;
    return CO_OK;
}

int
oseen2_fin(T * q)
{
    FREE(q);
    return CO_OK;
}

int
oseen2_apply(T * q, Skel * skel, const real * x, const real * y,
             real * oxx, real * oxy, real * oyy)
{
    int n, i;
    real e, s;

    n = skel_nv(skel);
    e = q->e;
#pragma omp parallel for
    for (i = 0; i < n; i++) {
        int j;
        real a[2], b[2], xx, xy, yy;

        vec2_get(i, x, y, a);
        oseen0(e, &xx, &xy, &yy);
        matrix_set(n, n, i, i, xx, oxx);
        matrix_set(n, n, i, i, xy, oxy);
        matrix_set(n, n, i, i, yy, oyy);
        for (j = 0; j < n; j++) {
            if (i == j)
                continue;
            vec2_get(j, x, y, b);
            oseen(e, a, b, &xx, &xy, &yy);
            matrix_set(n, n, i, j, xx, oxx);
            matrix_set(n, n, i, j, xy, oxy);
            matrix_set(n, n, i, j, yy, oyy);
        }
    }
    s = 1 / (4 * pi);
    matrix_scale(n, n, s, oxx);
    matrix_scale(n, n, s, oxy);
    matrix_scale(n, n, s, oyy);
    return CO_OK;
}

int
oseen2_stresslet(T * q, Skel * skel, const real * x, const real * y,
                 real * oxx, real * oxy, real * oyy)
{
    USED(q);
    USED(skel);
    USED(x);
    USED(y);
    USED(oxx);
    USED(oxy);
    USED(oyy);
    return CO_OK;
}

real
oseen2_pressure(T * q, Skel * skel, const real * x, const real * y,
                const real * fx, const real * fy, const real r[2])
{
    USED(q);
    int n, i;
    real p, s, a[2], f[2], d[2];
    Sum *sum;

    n = skel_nv(skel);
    sum_ini(&sum);
    for (i = 0; i < n; i++) {
        vec2_get(i, x, y, a);
        vec2_get(i, fx, fy, f);
        vec2_minus(r, a, d);
        sum_add(sum, vec2_dot(f, d) / vec2_dot(d, d));
    }
    p = sum_get(sum);
    sum_fin(sum);
    s = 1 / (2 * pi);
    return s * p;
}

int
oseen2_velocity(T * q, Skel * skel, real mu, const real * x,
                const real * y, const real * fx, const real * fy,
                const real r[2], real u[2])
{
    USED(q);
    int n, i;
    real s, rk, rk2, l, a[2], f[2], d[2];

    n = skel_nv(skel);
    vec2_zero(u);
    for (i = 0; i < n; i++) {
        vec2_get(i, x, y, a);
        vec2_get(i, fx, fy, f);
        vec2_minus(r, a, d);
        rk = vec2_abs(d);
        rk2 = rk * rk;
        l = log(rk);
        vec2_scalar_add(f, -l, u);
        vec2_scalar_add(d, vec2_dot(f, d) / rk2, u);
    }
    s = 1 / (4 * pi * mu);
    vec2_scale(s, u);
    return CO_OK;;
}
