#include <stdio.h>
#include <math.h>
#include "real.h"
#include "co/argv.h"
#include "co/dedg2.h"
#include "co/dtri2.h"
#include "co/edg2.h"
#include "co/err.h"
#include "co/f2/bend_ade.h"
#include "co/len.h"
#include "co/memory.h"
#include "co/predicate.h"
#include "co/skel.h"
#include "co/sum.h"
#include "co/tri2.h"
#include "co/vec2.h"

#define T F2BendAde
#define FMT CO_REAL_OUT
static const real pi = 3.141592653589793115997964;

struct T {
    int n;
    real k, DA0D;
};

int
f2_bend_ade_ini(real k, real DA0D, Skel * skel, T ** pq)
{
    T *q;
    int n;

    MALLOC(1, &q);
    predicate_ini();
    n = skel_ne(skel);
    q->n = n;
    q->k = k;
    q->DA0D = DA0D;
    *pq = q;
    return CO_OK;
}

int
f2_bend_ade_argv(char ***p, Skel * skel, T ** pq)
{
    int status;
    real x, y;

    if ((status = argv_real(p, &x)) != CO_OK)
        return status;
    if ((status = argv_real(p, &y)) != CO_OK)
        return status;
    return f2_bend_ade_ini(x, y, skel, pq);
}

int
f2_bend_ade_fin(T * q)
{
    FREE(q);
    return CO_OK;
}

static real
compute_p(Skel * skel, const real * x, const real * y)
{
    int v, i, j, k, n;
    HeSum *sum;
    real a[2], b[2], c[2], p, P;

    n = skel_nv(skel);
    sum_ini(&sum);
    for (v = 0; v < n; v++) {
        if (skel_bnd(skel, v))
            continue;
        skel_ver_ijk(skel, v, &i, &j, &k);
        vec2_get(i, x, y, a);
        vec2_get(j, x, y, b);
        vec2_get(k, x, y, c);
        p = tri2_angle_sup(a, b, c);
        sum_add(sum, p);
    }
    P = sum_get(sum);
    sum_fin(sum);
    return P;
}

real
f2_bend_ade_energy(T * q, Skel * skel, const real * x, const real * y)
{
    real D, k, DA0D, P;

    k = q->k;
    DA0D = q->DA0D;
    P = compute_p(skel, x, y);
    D = P - DA0D;
    return k * D * D;
}

int
f2_bend_ade_force(T * q, Skel * skel, const real * x, const real * y,
                  real * fx, real * fy)
{
    int v, i, j, k, n;
    real a[2], b[2], c[2], da[2], db[2], dc[2], k0, P, DA0D;
    real coeff;

    n = skel_nv(skel);
    k0 = q->k;
    DA0D = q->DA0D;
    P = compute_p(skel, x, y);
    coeff = k0 * (P - DA0D);
    MSG("P " FMT, P / (2 * pi));
    for (v = 0; v < n; v++) {
        if (skel_bnd(skel, v))
            continue;
        skel_ver_ijk(skel, v, &i, &j, &k);
        vec2_get(i, x, y, a);
        vec2_get(j, x, y, b);
        vec2_get(k, x, y, c);
        if (dtri2_angle_sup(a, b, c, da, db, dc) != CO_OK)
            ERR(CO_NUM, "dtri2_angle_sup failed for ijk: %d %d %d", i, j,
                k);
        vec2_scalar_append(da, coeff, i, fx, fy);
        vec2_scalar_append(db, coeff, j, fx, fy);
        vec2_scalar_append(dc, coeff, k, fx, fy);
    }
    return CO_OK;
}
