#include <stdio.h>

#include "real.h"
#include "he/memory.h"
#include "he/err.h"
#include "he/he.h"
#include "he/vec.h"
#include "he/edg.h"
#include "he/dedg.h"

#include "he/f/edg_sq.h"

#define T HeFEdgSq

#    define  nxt(h)     he_nxt(he, h)
#    define  flp(h)     he_flp(he, h)
#    define  ver(h)     he_ver(he, h)
#    define  hdg_ver(v) he_hdg_ver(he, v)
#    define  hdg_edg(e) he_hdg_edg(he, e)
#    define  hdg_tri(t) he_hdg_tri(he, t)
#    define  bnd(h)     he_bnd(he, h)

static const real pi = 3.141592653589793115997964;

struct T {
    int ne, nv;
    real *edg;
    int *rank;
    real K, cutoff;
};

static real sum_sq(int n, real *a) {
    int i;
    real v;
    v = 0;
    for (i = 0; i < n; i++) v += a[i]*a[i];
    return v;
}
int he_f_edg_sq_ini(real K, real cutoff, He *he, T **pq) {
    T *q;
    int nv, ne;
    MALLOC(1, &q);
    ne = he_ne(he);
    nv = he_nv(he);

    MALLOC(ne, &q->edg);
    MALLOC(nv, &q->rank);

    q->ne = ne;
    q->nv = nv;
    q->K = K;
    q->cutoff = cutoff;

    *pq = q;
    return HE_OK;
}

int he_f_edg_sq_fin(T *q) {
    FREE(q->edg);
    FREE(q->rank);
    FREE(q);
    return HE_OK;
}

int he_f_edg_sq_e(T *q, /**/ real  **pa) {
    *pa = q->edg;
    return HE_OK;
}

enum {BULK, BND};
static int get_ijkl(int h, He *he, /**/ int *pi, int *pj, int *pk, int *pl) {
    int n, nn, nnf, i, j, k, l;
    if (bnd(h)) return BND;

    n = nxt(h); nn = nxt(nxt(h));
    nnf = nxt(nxt(flp(h)));
    j = ver(h); k = ver(n); i = ver(nn); l = ver(nnf);

    *pi = i; *pj = j; *pk = k; *pl = l;
    return BULK;
}
static int get_ij(int m, He *he, /**/ int *pi, int *pj) {
    int h, n, i, j;
    h = hdg_edg(m);
    n = nxt(h);
    i = ver(h); j = ver(n);
    *pi = i; *pj = j;
    return HE_OK;
}
static int get(int m, He *he,
                const real *x, const real *y, const real *z, /**/
                real a[3], real b[3]) {
    int i, j;
    get_ij(m, he, &i, &j);
    vec_get(i, x, y, z, /**/ a);
    vec_get(j, x, y, z, /**/ b);
    return HE_OK;
}
static int compute_edg(He *he, const real *x, const real *y, const real *z, /**/ real *edg) {
    real one, a[3], b[3];
    int n, m;
    n = he_ne(he);
    for (m = 0; m < n; m++) {
        get(m, he, x, y, z, /**/ a, b);
        edg[m]  = one = edg_abs(a, b);
    }
    return HE_OK;
}

static real dw0(real s) {
    if (s < 1) return 3*s*s - 6;
    else if (s < 2) return -3*(2-s)*(2-s);
    else return 0;
}
static real dw(real h, real s) { return 1/(3*pi*h*h)*dw0(2*s/h); }
static int zero(int n, int a[]) {
    int i;
    for (i = 0; i < n; i++)
        a[i] = 0;
}
static int sq(int x) { return x*x; }
static int force(real K, real cutoff, int i, int j,
                 const int *rank,
                 const real *x, const real *y, const real *z, /**/
                 real *fx, real *fy, real *fz) {
    real a[3], b[3], da[3], db[3], edg, coeff;
    vec_get(i, x, y, z, /**/ a);
    vec_get(j, x, y, z, /**/ b);
    dedg_abs(a, b, /**/ da, db);
    edg = edg_abs(a, b);
    coeff = 2*K*dw(cutoff, edg) * (1.0/rank[i] + 1.0/rank[j]);
    vec_scalar_append(da, coeff, i, /**/ fx, fy, fz);
}
static int compute_force(He *he, real K, real cutoff, int const *rank,
                         const real *x, const real *y, const real *z, /**/
                         real *fx, real *fy, real *fz) {
    int nh, h, i, j, k, l;
    nh = he_nh(he);
    for (h = 0; h < nh; h++) {
        get_ijkl(h, he, /**/ &i, &j, &k, &l);
        force(K, cutoff, j, k, rank, x, y, z, fx, fy, fz);
        force(K, cutoff, i, l, rank, x, y, z, fx, fy, fz);
    }
    return HE_OK;
}

static int compute_rank(He *he, /**/ int *rank) {
    int m, nv, ne, i, j;
    nv = he_nv(he);
    ne = he_ne(he);
    zero(nv, rank);
    for (m = 0; m < ne; m++) {
        get_ij(m, he, &i, &j);
        rank[i] += 1;
        rank[j] += 1;
    }
    return HE_OK;
}

int he_f_edg_sq_force(T *q, He *he,
                      const real *x, const real *y, const real *z, /**/
                      real *fx, real *fy, real *fz) {
    int nv, ne;
    real *edg, K, cutoff;
    int *rank;

    ne = q->ne;
    nv = q->nv;

    edg = q->edg;
    rank = q->rank;

    K  = q->K;
    cutoff = q->cutoff;

    if (he_ne(he) != ne)
        ERR(HE_INDEX, "he_ne(he)=%d != ne = %d", he_ne(he), ne);
    if (he_nv(he) != nv)
        ERR(HE_INDEX, "he_nv(he)=%d != nv = %d", he_nv(he), nv);

    compute_rank(he, /**/ rank);
    compute_edg(he, x, y, z, /**/ edg);
    compute_force(he, K, cutoff, rank, x, y, z, /**/ fx, fy, fz);
    return HE_OK;
}

real he_f_edg_sq_energy(T *q, He *he,
                      const real *x, const real *y, const real *z) {
    int ne;
    real *edg, v, K;
    ne = q->ne;
    edg = q->edg;
    K  = q->K;

    if (he_ne(he) != ne)
        ERR(HE_INDEX, "he_ne(he)=%d != ne = %d", he_ne(he), ne);

    compute_edg(he, x, y, z, /**/ edg);
    v = sum_sq(ne, edg);
    return K*v;
}
