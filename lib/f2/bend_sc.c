#include <stdio.h>
#include <math.h>
#include "real.h"
#include "co/argv.h"
#include "co/dedg2.h"
#include "co/dtri2.h"
#include "co/edg2.h"
#include "co/err.h"
#include "co/f2/bend_sc.h"
#include "co/memory.h"
#include "co/predicate.h"
#include "co/skel.h"
#include "co/sum.h"
#include "co/tri2.h"
#include "co/vec2.h"

#define T F2BendSc
#define FMT CO_REAL_OUT

struct T {
    int n;
    real k, H0;
};

int
f2_bend_sc_ini(real k, real H0, Skel * skel, T ** pq)
{
    T *q;
    int n;

    MALLOC(1, &q);
    predicate_ini();
    n = skel_ne(skel);
    q->n = n;
    q->k = k;
    q->H0 = H0;
    *pq = q;
    return CO_OK;
}

int
f2_bend_sc_argv(char ***p, Skel * skel, T ** pq)
{
    int status;
    real x, y;

    if ((status = argv_real(p, &x)) != CO_OK)
        return status;
    if ((status = argv_real(p, &y)) != CO_OK)
        return status;
    return f2_bend_sc_ini(x, y, skel, pq);
}

int
f2_bend_sc_fin(T * q)
{
    FREE(q);
    return CO_OK;
}

static real
compute_energy(real H0, Skel * skel, const real * x, const real * y)
{
    int v, i, j, k, n;
    Sum *sum;
    real a[2], b[2], c[2], u, w, p, h, E, k0, dh;

    n = skel_nv(skel);
    sum_ini(&sum);
    for (v = 0; v < n; v++) {
        if (skel_bnd(skel, v))
            continue;
        skel_ver_ijk(skel, v, &i, &j, &k);
        vec2_get(i, x, y, a);
        vec2_get(j, x, y, b);
        vec2_get(k, x, y, c);
        u = edg2_abs(a, b);
        w = edg2_abs(b, c);
        if (u + w == 0)
            ERR(CO_NUM, "u + w == 0");
        p = tri2_angle_sup(a, b, c);
        h = p / (u + w);
        dh = h - H0;
        sum_add(sum, dh * dh);
    }
    E = sum_get(sum);
    sum_fin(sum);
    return E;
}

real
f2_bend_sc_energy(T * q, Skel * skel, const real * x, const real * y)
{
    real E, k, H0;

    k = q->k;
    H0 = q->H0;
    E = compute_energy(H0, skel, x, y);
    return k * E;
}

static real
sq(real x)
{
    return x * x;
}

int
f2_bend_sc_force(T * q, Skel * skel, const real * x, const real * y,
                 real * fx, real * fy)
{
    int v, i, j, k, n;
    real a[2], b[2], c[2], da[2], db[2], dc[2], u, w, h, dh, k0, H0;
    real p, coeff;

    n = skel_nv(skel);
    k0 = q->k;
    H0 = q->H0;
    for (v = 0; v < n; v++) {
        if (skel_bnd(skel, v))
            continue;
        skel_ver_ijk(skel, v, &i, &j, &k);
        vec2_get(i, x, y, a);
        vec2_get(j, x, y, b);
        vec2_get(k, x, y, c);
        u = edg2_abs(a, b);
        w = edg2_abs(b, c);
        p = tri2_angle_sup(a, b, c);
        if (u + w == 0)
            ERR(CO_NUM, "u + w == 0");
        h = p / (u + w);
        dh = h - H0;
        if (dtri2_angle_sup(a, b, c, da, db, dc) != CO_OK)
            ERR(CO_NUM, "dtri2_angle_sup failed for ijk: %d %d %d", i, j,
                k);
        coeff = 2 * k0 * dh / (u + w);
        vec2_scalar_append(da, coeff, i, fx, fy);
        vec2_scalar_append(db, coeff, j, fx, fy);
        vec2_scalar_append(dc, coeff, k, fx, fy);

        dedg2_abs(a, b, da, db);
        coeff = -2 * k0 * dh * p / sq(u + w);
        vec2_scalar_append(da, coeff, i, fx, fy);
        vec2_scalar_append(db, coeff, j, fx, fy);

        dedg2_abs(c, b, dc, db);
        vec2_scalar_append(dc, coeff, k, fx, fy);
        vec2_scalar_append(db, coeff, j, fx, fy);
    }
    return CO_OK;
}
