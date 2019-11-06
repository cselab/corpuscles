#include <stdio.h>
#include <math.h>

#include "real.h"
#include "co/argv.h"
#include "co/dvec2.h"
#include "co/err.h"
#include "co/f2/area.h"
#include "co/memory.h"
#include "co/skel.h"
#include "co/sum.h"
#include "co/vec2.h"

#define T F2Area
#define FMT CO_REAL_OUT

struct T {
    int n;
    real a, k, A;
};

int
f2_area_ini(real a, real k, Skel * skel, T ** pq)
{
    T *q;
    int n;

    if (a <= 0)
        ERR(CO_NUM, "a <= 0");
    MALLOC(1, &q);
    n = skel_ne(skel);
    q->n = n;
    q->a = a;
    q->k = k;
    *pq = q;
    return CO_OK;
}

int
f2_area_argv(char ***p, Skel * skel, T ** pq)
{
    int status;
    real x, y;

    if ((status = argv_real(p, &x)) != CO_OK)
        return status;
    if ((status = argv_real(p, &y)) != CO_OK)
        return status;
    return f2_area_ini(x, y, skel, pq);
}

int
f2_area_fin(T * q)
{
    FREE(q);
    return CO_OK;
}

real
compute_area(Skel * skel, const real * x, const real * y)
{
    int e, i, j, n;
    HeSum *sum;
    real a[2], b[2], A;

    n = skel_ne(skel);
    he_sum_ini(&sum);
    for (e = 0; e < n; e++) {
        skel_edg_ij(skel, e, &i, &j);
        vec2_get(i, x, y, a);
        vec2_get(j, x, y, b);
        he_sum_add(sum, vec2_cross(b, a));
    }
    A = he_sum_get(sum);
    A = fabs(A);
    he_sum_fin(sum);
    return A / 2;
}

real
f2_area_energy(T * q, Skel * skel, const real * x, const real * y)
{
    real A, k, a;

    a = q->a;
    k = q->k;
    A = compute_area(skel, x, y);
    q->A = A;
    return k * (A - a) * (A - a) / a;
}

int
f2_area_force(T * q, Skel * skel, const real * x, const real * y, real * u,
              real * v)
{
    int e, i, j, n;
    real A, k, a0, coeff;
    real a[2], b[2], da[2], db[2];

    n = skel_ne(skel);
    a0 = q->a;
    k = q->k;
    A = compute_area(skel, x, y);
    coeff = k * (A - a0) / a0;
    for (e = 0; e < n; e++) {
        skel_edg_ij(skel, e, &i, &j);
        vec2_get(i, x, y, a);
        vec2_get(j, x, y, b);
        dvec2_cross(b, a, db, da);
        vec2_scalar_append(da, coeff, i, u, v);
        vec2_scalar_append(db, coeff, j, u, v);
    }
    return CO_OK;
}

real
f2_area_area(T * q)
{
    return q->A;
}
