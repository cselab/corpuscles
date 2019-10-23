#include <stdio.h>

#include "real.h"
#include "co/argv.h"
#include "co/memory.h"
#include "co/err.h"
#include "co/he.h"
#include "co/vec.h"
#include "co/tri.h"
#include "co/dtri.h"

#include "co/f/area.h"

#define T HeFArea

struct T {
    int n;
    real *area, *darea;
    real a0, K;
};

int
he_f_area_ini(real a0, real K, He * he, T ** pq)
{
    T *q;
    int n;

    MALLOC(1, &q);
    n = he_nt(he);

    MALLOC(n, &q->darea);
    MALLOC(n, &q->area);

    q->n = n;
    q->a0 = a0;
    q->K = K;

    *pq = q;
    return CO_OK;
}

int
he_f_area_argv(char ***p, He * he, T ** pq)
{
    int status;
    real x, y;

    if ((status = argv_real(p, &x)) != CO_OK)
        return status;
    if ((status = argv_real(p, &y)) != CO_OK)
        return status;
    return he_f_area_ini(x, y, he, pq);
}

int
he_f_area_fin(T * q)
{
    FREE(q->area);
    FREE(q->darea);
    FREE(q);
    return CO_OK;
}

int
he_f_area_a(T * q, /**/ real ** pa)
{
    *pa = q->area;
    return CO_OK;
}

int
he_f_area_da(T * q, /**/ real ** pa)
{
    *pa = q->darea;
    return CO_OK;
}

static void
get(int t, He * he,
    const real * x, const real * y, const real * z, /**/
    real a[3], real b[3], real c[3])
{
    int i, j, k;

    he_tri_ijk(he, t, &i, &j, &k);
    vec_get(i, x, y, z, /**/ a);
    vec_get(j, x, y, z, /**/ b);
    vec_get(k, x, y, z, /**/ c);
}

static void
compute_area(real a0, He * he, const real * x, const real * y,
             const real * z, /**/ real * area, real * darea)
{
    real one, a[3], b[3], c[3];
    int n, t;

    n = he_nt(he);
    for (t = 0; t < n; t++) {
        get(t, he, x, y, z, /**/ a, b, c);
        area[t] = one = tri_area(a, b, c);
        darea[t] = one - a0;
    }
}

static void
compute_force(real K, real a0, real * darea, He * he, const real * x,
              const real * y, const real * z, /**/ real * fx, real * fy,
              real * fz)
{
    int n, t, i, j, k;
    real a[3], b[3], c[3], da[3], db[3], dc[3], coeff;

    n = he_nt(he);
    for (t = 0; t < n; t++) {
        he_tri_ijk(he, t, /**/ &i, &j, &k);
        vec_get(i, x, y, z, /**/ a);
        vec_get(j, x, y, z, /**/ b);
        vec_get(k, x, y, z, /**/ c);
        dtri_area(a, b, c, /**/ da, db, dc);
        coeff = a0 == 0 ? 2 * K * darea[t] : 2 * K * darea[t] / a0;
        vec_scalar_append(da, coeff, i, /**/ fx, fy, fz);
        vec_scalar_append(db, coeff, j, /**/ fx, fy, fz);
        vec_scalar_append(dc, coeff, k, /**/ fx, fy, fz);
    }
}

int
he_f_area_force(T * q, He * he,
                const real * x, const real * y, const real * z, /**/
                real * fx, real * fy, real * fz)
{
    int n;
    real *area, *darea, a0, K;

    n = q->n;
    area = q->area;
    darea = q->darea;
    a0 = q->a0;
    K = q->K;

    if (he_nt(he) != n)
        ERR(CO_INDEX, "he_nt(he)=%d != n = %d", he_nt(he), n);

    compute_area(a0, he, x, y, z, /**/ area, darea);
    compute_force(K, a0, darea, he, x, y, z, /**/ fx, fy, fz);

    return CO_OK;
}

static real
compute_energy(real K, real a0, real * darea, int n)
{
    int t;
    real da, e;

    e = 0;
    for (t = 0; t < n; t++) {
        da = darea[t];
        e += da * da;
    }
    return (a0 == 0) ? K * e : K * e / a0;
}

real
he_f_area_energy(T * q, He * he,
                 const real * x, const real * y, const real * z)
{
    int n;
    real *area, *darea, a0, K;

    n = q->n;
    area = q->area;
    darea = q->darea;
    a0 = q->a0;
    K = q->K;

    if (he_nt(he) != n)
        ERR(CO_INDEX, "he_nt(he)=%d != n = %d", he_nt(he), n);

    compute_area(a0, he, x, y, z, /**/ area, darea);
    return compute_energy(K, a0, darea, n);
}
