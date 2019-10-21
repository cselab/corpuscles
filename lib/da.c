#include <stdio.h>

#include "real.h"

#include "co/dedg.h"
#include "co/dtri.h"
#include "co/edg.h"
#include "co/err.h"
#include "co/he.h"
#include "co/memory.h"
#include "co/tri.h"
#include "co/vec.h"

#include "co/da.h"

#define T Da

#define BEGIN_HE                                 \
    nh = he_nh(he);                              \
    for (h = 0; h < nh; h++) {                   \
    he_ijk(he, h, &i, &j, &k);                   \
    vec_get(i, x, y, z, a);                      \
    vec_get(j, x, y, z, b);                      \
    vec_get(k, x, y, z, c);
#define END_HE }

#define BEGIN_VER                                \
    nv = he_nv(he);                              \
    for (i = 0; i < nv; i++) {
#define END_VER }

typedef struct Vec Vec;
struct Vec {
    real v[3];
};

struct T {
    int nv, nh;
    real *tb, *tc, *sb, *sc;
    real *dda, *area;
    Vec *f;
};

int
da_ini(He * he, /**/ T ** pq)
{
#define M(n, f) MALLOC(n, &q->f)
    int nv, nh;
    T *q;

    MALLOC(1, &q);
    nv = he_nv(he);
    nh = he_nh(he);

    M(nh, tb);
    M(nh, tc);
    M(nh, sb);
    M(nh, sc);
    M(nv, dda);
    M(nv, area);
    M(nv, f);

    q->nv = nv;
    q->nh = nh;
    *pq = q;
    return CO_OK;
#undef M
}

int
da_fin(T * q)
{
#define F(x) FREE(q->x)
    F(tb);
    F(tc);
    F(sb);
    F(sc);
    F(dda);
    F(area);
    F(f);

    return CO_OK;
#undef F
}

int
da_compute_area(T * q, He * he, const real * x, const real * y,
                const real * z)
{
#define G(f) f = q->f
    int nh, nv, h, i, j, k;
    real a[3], b[3], c[3];

    real *tb, *tc, *sb, *sc;
    real *area;

    G(tb);
    G(tc);
    G(sb);
    G(sc);
    G(area);

    nh = he_nh(he);
    nv = he_nv(he);

    BEGIN_VER {
        area[i] = 0;
    } END_VER;

    BEGIN_HE {
        tb[h] = tri_cot(a, b, c);
        tc[h] = tri_cot(b, c, a);
        sb[h] = edg_sq(a, b);
        sc[h] = edg_sq(a, c);
    } END_HE;

    BEGIN_HE {
        area[i] += (tb[h] * sc[h] + tc[h] * sb[h]) / 8;
    } END_HE;

    return CO_OK;
#undef A
}

int
da_force(T * q, dAParam param, He * he, const real * x, const real * y,
         const real * z, /**/ real * fx, real * fy, real * fz)
{
#define G(f) f = q->f
    int nh, nv, h, i, j, k;
    real a[3], b[3], c[3];
    const real *v;

    real *tb, *tc, *sb, *sc;
    real *dda, *area;
    Vec *f;

    real da[3], db[3], dc[3];
    real C;
    void *p;

    real(*DA) (void *, real);

    DA = param.da;
    p = param.p;

    G(tb);
    G(tc);
    G(sb);
    G(sc);
    G(dda);
    G(area);
    G(f);

    nh = he_nh(he);
    nv = he_nv(he);

    BEGIN_VER {
        area[i] = 0;
        vec_zero(f[i].v);
    } END_VER;

    BEGIN_HE {
        tb[h] = tri_cot(a, b, c);
        tc[h] = tri_cot(b, c, a);

        sb[h] = edg_sq(a, b);
        sc[h] = edg_sq(a, c);
    } END_HE;

    BEGIN_HE {
        area[i] += (tb[h] * sc[h] + tc[h] * sb[h]) / 8;
    } END_HE;

    BEGIN_VER {
        dda[i] = DA(p, area[i]);
    } END_VER;

    BEGIN_HE {
        dtri_cot(a, b, c, da, db, dc);
        C = dda[i] * sc[h] / 8;
        vec_axpy(C, da, f[i].v);
        vec_axpy(C, db, f[j].v);
        vec_axpy(C, dc, f[k].v);
    } END_HE;

    BEGIN_HE {
        dtri_cot(b, c, a, db, dc, da);
        C = dda[i] * sb[h] / 8;
        vec_axpy(C, da, f[i].v);
        vec_axpy(C, db, f[j].v);
        vec_axpy(C, dc, f[k].v);
    } END_HE;

    BEGIN_HE {
        dedg_sq(a, b, da, db);
        C = dda[i] * tc[h] / 8;
        vec_axpy(C, da, f[i].v);
        vec_axpy(C, db, f[j].v);
    } END_HE;

    BEGIN_HE {
        dedg_sq(a, c, da, dc);
        C = dda[i] * tb[h] / 8;
        vec_axpy(C, da, f[i].v);
        vec_axpy(C, dc, f[k].v);
    } END_HE;

    BEGIN_VER {
        v = f[i].v;
        vec_append(v, i, fx, fy, fz);
    } END_VER;

    return CO_OK;
#undef A
}


int
da_area(T * q, real ** parea)
{
    *parea = q->area;
    return CO_OK;
}
