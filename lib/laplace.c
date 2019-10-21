#include <tgmath.h>
#include <stdio.h>

#include "real.h"
#include "co/err.h"
#include "co/he.h"
#include "co/vec.h"
#include "co/dvec.h"
#include "co/tri.h"
#include "co/dtri.h"
#include "co/ten.h"
#include "co/edg.h"
#include "co/memory.h"
#include "co/laplace.h"

#define T Laplace

#define BEGIN_LOOP                               \
    nh = he_nh(he);                              \
    for (h = 0; h < nh; h++) {                   \
    he_ijk(he, h, &i, &j, &k);                   \
    vec_get(i, x, y, z, a);                      \
    vec_get(j, x, y, z, b);                      \
    vec_get(k, x, y, z, c);
#define END_LOOP }

enum { X, Y, Z };

typedef struct Vec Vec;
struct Vec {
    real v[3];
};

struct T {
    int nv, nh;
    real *tb, *tc, *sb, *sc;
    Vec *eb, *ec, *lp;
    real *area, *lx, *ly, *lz;
};

int
laplace_ini(He * he, /**/ T ** pq)
{
    int nv, nh;
    T *q;

    MALLOC(1, &q);

    nv = he_nv(he);
    nh = he_nh(he);

    MALLOC(nh, &q->tb);
    MALLOC(nh, &q->tc);
    MALLOC(nh, &q->eb);
    MALLOC(nh, &q->ec);

    MALLOC(nh, &q->sb);
    MALLOC(nh, &q->sc);

    MALLOC(nv, &q->area);
    MALLOC(nv, &q->lp);

    MALLOC(nv, &q->lx);
    MALLOC(nv, &q->ly);
    MALLOC(nv, &q->lz);

    q->nv = nv;
    q->nh = nh;
    *pq = q;
    return CO_OK;
}

int
laplace_fin(T * q)
{
    FREE(q->tb);
    FREE(q->tc);
    FREE(q->eb);
    FREE(q->ec);
    FREE(q->sb);
    FREE(q->sc);
    FREE(q->area);
    FREE(q->lp);
    FREE(q->lx);
    FREE(q->ly);
    FREE(q->lz);
    FREE(q);
    return CO_OK;
}

int
laplace_apply(T * q, He * he, const real * x, const real * y,
              const real * z, /**/ real ** plx, real ** ply, real ** plz,
              real ** parea)
{
    int nh, nv, h, i, j, k;
    real a[3], b[3], c[3];
    real *tb, *tc, *sb, *sc, *area;
    Vec *eb, *ec, *lp;
    real *lx, *ly, *lz;

    tb = q->tb;
    tc = q->tc;
    sb = q->sb;
    sc = q->sc;

    eb = q->eb;
    ec = q->ec;

    area = q->area;
    lp = q->lp;

    lx = q->lx;
    ly = q->ly;
    lz = q->lz;

    nv = he_nv(he);
    for (i = 0; i < nv; i++) {
        area[i] = 0;
        vec_zero(lp[i].v);
    }

    BEGIN_LOOP {
        tb[h] = tri_cot(a, b, c);
        tc[h] = tri_cot(b, c, a);

        vec_minus(a, b, eb[h].v);
        vec_minus(a, c, ec[h].v);

        sb[h] = edg_sq(a, b);
        sc[h] = edg_sq(a, c);
    }
    END_LOOP;

    BEGIN_LOOP {
        vec_axpy(tb[h], ec[h].v, /**/ lp[i].v);
        vec_axpy(tc[h], eb[h].v, /**/ lp[i].v);
        area[i] += tb[h] * sc[h] + tc[h] * sb[h];
    }
    END_LOOP;

    for (i = 0; i < nv; i++) {
        lx[i] = lp[i].v[X];
        ly[i] = lp[i].v[Y];
        lz[i] = lp[i].v[Z];
    }

    *plx = lx;
    *ply = ly;
    *plz = lz;
    *parea = area;
    return CO_OK;
}
