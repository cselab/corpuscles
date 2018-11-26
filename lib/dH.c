#include <math.h>
#include <stdio.h>

#include "real.h"
#include "he/err.h"
#include "he/he.h"
#include "he/vec.h"
#include "he/dvec.h"
#include "he/tri.h"
#include "he/edg.h"
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
    real *H, *area;
    Vec *eb, *ec, *u, *lp, *m, *n, *ldn;
};

int dh_ini(He *he, /**/ T **pq) {
#   define M(n, f) MALLOC(n, &q->f)
    int nv, nh;
    T *q;
    MALLOC(1, &q);
    nv = he_nv(he);
    nh = he_nh(he);

    M(nh, tb);
    M(nh, tc);
    M(nh, sb);
    M(nh, sc);
    M(nh, ang);

    M(nv, H);
    M(nv, area);

    M(nh, eb);
    M(nh, ec);
    M(nh, u);

    M(nv, lp);
    M(nv, m);
    M(nv, n);
    M(nv, ldn);

    q->nv = nv;
    q->nh = nh;
    *pq = q;
    return HE_OK;
#   undef M
}

int dh_fin(T *q) {
#   define F(x) FREE(q->x)
    F(tb); F(tc); F(sb); F(sc); F(ang);
    F(H); F(area);
    F(eb); F(ec); F(u); F(lp); F(m); F(n); F(ldn);
    return HE_OK;
#   undef F
}

static int QplusAbc(const Ten *A, const Vec b, const real c[3],
                    /**/ Ten *Q) {
    real x[3];
    Ten Y;
    ten_vec(A, b.v, /**/ x);
    ten_dyadic(x, c, /**/ &Y);
    ten_add(&Y, Q);
    return HE_OK;
}
int dh_apply(T *q, He *he, const real *x, const real *y, const real *z, /**/ real *fx, real *fy, real *fz) {
#   define A(f) f = q->f
    int nh, nv, h, i, j, k;
    real a[3], b[3], c[3];

    real *tb, *tc, *sb, *sc, *ang;
    real *H, *area;
    Vec *eb, *ec, *u, *lp, *m, *n, *ldn;
    Ten Dn;

    A(tb); A(tc); A(sb); A(sc); A(ang);
    A(H); A(area);
    A(eb); A(ec); A(u); A(lp); A(m); A(n); A(ldn);

    nh = he_nh(he);
    nv = he_nv(he);

    MSG("nv: %d", nv);

    BEGIN_VER {
        vec_zero(m[i].v);
        vec_zero(lp[i].v);
        area[i] = 0;
    } END_VER;

    BEGIN_HE {
        tb[h] = tri_cot(a, b, c);
        tc[h] = tri_cot(b, c, a);
        vec_minus(a, b,   eb[h].v);
        vec_minus(a, c,   ec[h].v);

        sb[h] = edg_sq(a, b);
        sc[h] = edg_sq(a, c);

        tri_normal(a, b, c,   u[h].v);
        ang[h] = tri_angle(c, a, b);
    } END_HE;

    BEGIN_HE {
        vec_axpy(ang[h], u[h].v,   m[i].v);
        vec_axpy(tb[h], ec[h].v,   lp[i].v);
        vec_axpy(tc[h], eb[h].v,   lp[i].v);
        area[i] += tb[h]*sc[h] + tc[h]*sb[h];
    } END_HE;

    BEGIN_VER {
        vec_norm(m[i].v,  n[i].v);
        dvec_norm(m[i].v, &Dn);
        H[i] = vec_dot(lp[i].v, n[i].v);
        vec_ten(ldn[i].v, &Dn,   ldn[i].v);
    } END_VER;

    MSG("area: %g", area[0]);


    return HE_OK;
#   undef A
}
