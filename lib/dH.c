#include <math.h>
#include <stdio.h>

#include "real.h"
#include "he/err.h"
#include "he/he.h"
#include "he/vec.h"
#include "he/dvec.h"
#include "he/tri.h"
#include "he/dtri.h"
#include "he/ten.h"
#include "he/memory.h"
#include "he/dH.h"

#define T Dh

#define BEGIN_LOOP                               \
    nh = he_nh(he);                              \
    for (h = 0; h < nh; h++) {                   \
    he_ijk(he, h, &i, &j, &k);                   \
    vec_get(i, x, y, z, a);                      \
    vec_get(j, x, y, z, b);                      \
    vec_get(k, x, y, z, c);
#define END_LOOP }

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
    int nh, nv, h, i, j, k;
    real a[3], b[3], c[3];

    nh = he_nh(he);
    nv = he_nv(he);

    BEGIN_LOOP {
    } END_LOOP;

    return HE_OK;
}
