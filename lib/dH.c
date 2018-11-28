#include <tgmath.h>
#include <stdio.h>

#include "real.h"
#include "he/err.h"
#include "he/he.h"
#include "he/vec.h"
#include "he/dvec.h"
#include "he/tri.h"
#include "he/edg.h"
#include "he/dedg.h"
#include "he/dtri.h"
#include "he/ten.h"
#include "he/memory.h"
#include "he/dH.h"

#define T Dh

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
struct Vec { real v[3]; };

struct T {
    int nv, nh;
    real *tb, *tc, *sb, *sc, *ang;
    real *ddh, *dda, *H, *area;
    Vec *eb, *ec, *u, *lp, *m, *n, *ldn;
    Vec *f;
};

static int normal(const real a[3], const real b[3], const real c[3],
                  real u[3]) {
    tri_normal(a, b, c, u);
    return HE_OK;
}

int dh_ini(He *he, /**/ T **pq) {
#   define M(n, f) MALLOC(n, &q->f)
    int nv, nh;
    T *q;
    MALLOC(1, &q);
    nv = he_nv(he);
    nh = he_nh(he);

    M(nh, tb); M(nh, tc); M(nh, sb);
    M(nh, sc); M(nh, ang);
    M(nv, ddh); M(nv, dda); M(nv, H); M(nv, area);
    M(nh, eb); M(nh, ec); M(nh, u);
    M(nv, lp); M(nv, m); M(nv, n); M(nv, ldn); M(nv, f);

    q->nv = nv;
    q->nh = nh;
    *pq = q;
    return HE_OK;
#   undef M
}

int dh_fin(T *q) {
#   define F(x) FREE(q->x)
    F(tb); F(tc); F(sb); F(sc); F(ang);
    F(ddh); F(dda); F(H); F(area);
    F(eb); F(ec); F(u); F(lp); F(m); F(n); F(ldn);
    F(f);
    return HE_OK;
#   undef F
}

int dh_area_h(T *q, He *he, const real *x, const real *y, const real *z) {
#   define A(f) f = q->f
    int nh, nv, h, i, j, k;
    real a[3], b[3], c[3], n[3];

    real *tb, *tc, *sb, *sc, *ang;
    real *H, *area;
    Vec *eb, *ec, *u, *lp, *m;

    A(tb); A(tc); A(sb); A(sc); A(ang);
    A(H); A(area);
    A(eb); A(ec); A(u); A(lp); A(m);

    nh = he_nh(he);
    nv = he_nv(he);

    BEGIN_VER {
        vec_zero(m[i].v);
        vec_zero(lp[i].v);
        area[i] = 0;
    } END_VER;

    BEGIN_HE {
        tb[h] = tri_cot(a, b, c);
        tc[h] = tri_cot(b, c, a);
        vec_minus(b, a,   eb[h].v);
        vec_minus(c, a,   ec[h].v);

        sb[h] = edg_sq(a, b);
        sc[h] = edg_sq(a, c);

        tri_normal(a, b, c,   u[h].v);
        ang[h] = tri_angle(c, a, b);
    } END_HE;

    BEGIN_HE {
        vec_axpy(ang[h], u[h].v, m[i].v);
        vec_axpy(tb[h]/2, ec[h].v, lp[i].v);
        vec_axpy(tc[h]/2, eb[h].v, lp[i].v);
        area[i] += (tb[h]*sc[h] + tc[h]*sb[h])/8;
    } END_HE;

    BEGIN_VER {
        vec_norm(m[i].v,  n);
        H[i] = vec_dot(lp[i].v, n)/2;
    } END_VER;

    return HE_OK;
#   undef A
}

int dh_force(T *q, dHParam param, He *he, const real *x, const real *y, const real *z, /**/ real *fx, real *fy, real *fz) {
#   define A(f) f = q->f
    int nh, nv, h, i, j, k;
    real a[3], b[3], c[3];
    const real *v;

    real *tb, *tc, *sb, *sc, *ang;
    real*ddh, *dda, *H, *area;
    Vec *eb, *ec, *u, *lp, *m, *n, *ldn;
    Vec *f;

    Ten Dn, Da, Db, Dc;
    real da[3], db[3], dc[3];
    real C;
    void *p;
    real (*DH)(void*, real, real);
    real (*DA)(void*, real, real);

    DH = param.dh;
    DA = param.da;
    p = param.p;

    A(tb); A(tc); A(sb); A(sc); A(ang);
    A(ddh); A(dda); A(H); A(area);
    A(eb); A(ec); A(u); A(lp); A(m); A(n); A(ldn);
    A(f);

    nh = he_nh(he);
    nv = he_nv(he);

    BEGIN_VER {
        vec_zero(m[i].v);
        vec_zero(lp[i].v);
        vec_zero(f[i].v);
        area[i] = 0;
    } END_VER;

    BEGIN_HE {
        tb[h] = tri_cot(a, b, c);
        tc[h] = tri_cot(b, c, a);
        vec_minus(b, a,   eb[h].v);
        vec_minus(c, a,   ec[h].v);

        sb[h] = edg_sq(a, b);
        sc[h] = edg_sq(a, c);

        tri_normal(a, b, c,   u[h].v);
        ang[h] = tri_angle(c, a, b);
    } END_HE;

    BEGIN_HE {
        vec_axpy(ang[h], u[h].v,   m[i].v);
        vec_axpy(tb[h]/2, ec[h].v,   lp[i].v);
        vec_axpy(tc[h]/2, eb[h].v,   lp[i].v);
        area[i] += (tb[h]*sc[h] + tc[h]*sb[h])/8;
    } END_HE;

    BEGIN_VER {
        vec_norm(m[i].v,  n[i].v);
        H[i] = vec_dot(lp[i].v, n[i].v)/2;
        dvec_norm(m[i].v, &Dn);
        vec_ten(lp[i].v, &Dn,   ldn[i].v);
    } END_VER;

    BEGIN_VER {
        ddh[i] = DH(p, area[i], H[i]);
        dda[i] = DA(p, area[i], H[i]);
    } END_VER;

    BEGIN_HE {
        dtri_normal(a, b, c,   &Da, &Db, &Dc);
        C = ddh[i]*ang[h]/2;

        vec_ten(ldn[i].v, &Da,  da);
        vec_ten(ldn[i].v, &Db,  db);
        vec_ten(ldn[i].v, &Dc,  dc);

        vec_axpy(C, da, f[i].v);
        vec_axpy(C, db, f[j].v);
        vec_axpy(C, dc, f[k].v);
    } END_HE;

    BEGIN_HE {
        dtri_angle(c, a, b,  dc, da, db);
        C = ddh[i]*vec_dot(ldn[i].v, u[h].v)/2;
        vec_axpy(C, da, f[i].v);
        vec_axpy(C, db, f[j].v);
        vec_axpy(C, dc, f[k].v);
    } END_HE;

    BEGIN_HE {
        dtri_cot(a, b, c,  da, db, dc);
        C = dda[i]*sc[h]/8 + ddh[i]*vec_dot(n[i].v, ec[h].v)/4;
        vec_axpy(C, da, f[i].v);
        vec_axpy(C, db, f[j].v);
        vec_axpy(C, dc, f[k].v);
    } END_HE;

    BEGIN_HE {
        dtri_cot(b, c, a,  db, dc, da);
        C = dda[i]*sb[h]/8 + ddh[i]*vec_dot(n[i].v, eb[h].v)/4;
        vec_axpy(C, da, f[i].v);
        vec_axpy(C, db, f[j].v);
        vec_axpy(C, dc, f[k].v);
    } END_HE;

    BEGIN_HE {
        dedg_sq(a, b,  da, db);
        C = dda[i]*tc[h]/8;
        vec_axpy(C, da, f[i].v);
        vec_axpy(C, db, f[j].v);
    } END_HE;

    BEGIN_HE {
        dedg_sq(a, c,  da, dc);
        C = dda[i]*tb[h]/8;
        vec_axpy(C, da, f[i].v);
        vec_axpy(C, dc, f[k].v);
    } END_HE;

    BEGIN_HE {
        C = ddh[i]/4;
        vec_axpy(-C*(tc[h] + tb[h]), n[i].v, f[i].v);
        vec_axpy(C*tc[h], n[i].v, f[j].v);
        vec_axpy(C*tb[h], n[i].v, f[k].v);
    } END_HE;

    BEGIN_VER {
        v = f[i].v;
        vec_append(v, i, fx, fy, fz);
    } END_VER;

    return HE_OK;
#   undef A
}


int dh_area(T *q, real **parea) {
    *parea = q->area;
    return HE_OK;
}

int dh_h(T *q, real **pH) {
    *pH = q->H;
    return HE_OK;
}
