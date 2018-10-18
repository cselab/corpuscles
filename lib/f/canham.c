#include <stdio.h>
#include <math.h>

#include "real.h"
#include "he/macro.h"
#include "he/memory.h"
#include "he/err.h"
#include "he/he.h"
#include "he/vec.h"
#include "he/dih.h"
#include "he/ddih.h"
#include "he/tri.h"
#include "he/sum.h"

#include "he/f/canham.h"

#define T HeFCanham

#    define  nxt(h)     he_nxt(he, h)
#    define  flp(h)     he_flp(he, h)
#    define  ver(h)     he_ver(he, h)
#    define  hdg_ver(v) he_hdg_ver(he, v)
#    define  hdg_edg(e) he_hdg_edg(he, e)
#    define  hdg_tri(t) he_hdg_tri(he, t)
#    define  bnd(h)     he_bnd(he, h)

static const real pi = 3.141592653589793115997964;

struct T {
    real Kb;

    real *t;
    real *lbx, *lby, *lbz;
    real *normx, *normy, *normz;
    real *curva_mean, *curva_gauss;
    real *energy, *area, *lpl;
    real *fx, *fy, *fz;
    int nv, ne, nt;
};

static int zero(int n, real *a) {
    int i;
    for (i = 0; i < n; i++)
        a[i] = 0;
    return HE_OK;
}

static int plus(int n, const real *a, /*io*/ real *b) {
    int i;
    for (i = 0; i < n; i++)
        b[i] += a[i];
    return HE_OK;
}

static int scale(int n, real sc, /*io*/ real *a) {
    int i;
    for (i = 0; i < n; i++)
        a[i] *= sc;
    return HE_OK;
}

static int get_ijk(int t, He *he, /**/ int *pi, int *pj, int *pk) {
    int h, n, nn, i, j, k;
    h = hdg_tri(t);
    n = nxt(h);
    nn = nxt(n);
    i = ver(h); j = ver(n); k = ver(nn);
    *pi = i; *pj = j; *pk = k;
    return HE_OK;
}
static int get3(const real *x, const real *y, const real *z,
                int i, int j, int k,  /**/
                real a[3], real b[3], real c[3]) {
    vec_get(i, x, y, z, /**/ a);
    vec_get(j, x, y, z, /**/ b);
    vec_get(k, x, y, z, /**/ c);
    return HE_OK;
}
static real sum(int n, real *a) {
    int i;
    real v;
    HeSum *sum;
    he_sum_ini(&sum);
    v = 0;
    for (i = 0; i < n; i++)
        he_sum_add(sum, a[i]);
    v = he_sum_get(sum);
    he_sum_fin(sum);
    return v;
}

int he_f_canham_ini(real Kb, __UNUSED real C0, __UNUSED real Kad, __UNUSED real DA0D, He *he, T **pq) {
    T *q;
    int nv, ne, nt, nh;

    MALLOC(1, &q);

    nv = he_nv(he);
    ne = he_ne(he);
    nt = he_nt(he);
    nh = he_nh(he);

    q->nv = nv;
    q->ne = ne;
    q->nt = nt;

    q->Kb   = Kb;


    MALLOC(nh, &q->t); MALLOC(nh, &q->lpl);
    MALLOC(nv, &q->fx); MALLOC(nv, &q->fy); MALLOC(nv, &q->fz);
    MALLOC(nv, &q->lbx); MALLOC(nv, &q->lby); MALLOC(nv, &q->lbz);
    MALLOC(nv, &q->normx); MALLOC(nv, &q->normy); MALLOC(nv, &q->normz);
    MALLOC(nv, &q->curva_mean);  MALLOC(nv, &q->curva_gauss);
    MALLOC(nv, &q->energy); MALLOC(nv, &q->area);

    *pq = q;
    return HE_OK;
}
int he_f_canham_fin(T *q) {
    FREE(q->t);  FREE(q->lpl);
    FREE(q->fx); FREE(q->fy); FREE(q->fz);
    FREE(q->lbx); FREE(q->lby); FREE(q->lbz);
    FREE(q->normx);FREE(q->normy);FREE(q->normz);
    FREE(q->curva_mean);FREE(q->curva_gauss);
    FREE(q->energy); FREE(q->area);
    FREE(q);
    return HE_OK;
}
int he_f_canham_area_ver(T *q, /**/ real **pa) {
    *pa = q->area;
    return HE_OK;
}
int he_f_canham_laplace_ver(T *q, /**/ real **px, real **py, real **pz ) {
    *px = q->lbx;
    *py = q->lby;
    *pz = q->lbz;
    return HE_OK;
}
int he_f_canham_norm_ver(T *q, /**/ real **px, real **py, real **pz ) {
    *px = q->normx;
    *py = q->normy;
    *pz = q->normz;
    return HE_OK;
}
int he_f_canham_curva_mean_ver(T *q, /**/ real **pa) {
    *pa = q->curva_mean;
    return HE_OK;
}
int he_f_canham_curva_gauss_ver(T *q, /**/ real **pa) {
    *pa = q->curva_gauss;
    return HE_OK;
}
int he_f_canham_energy_ver(T *q, /**/ real**pa) {
    *pa = q->energy;
    return HE_OK;
}

static int compute_cot(He *he, const real *x, const real *y, const real *z, /**/ real *H) {
    int nh, h, n, nn;
    int i, j, k;
    real a[3], b[3], c[3], cot;
    nh = he_nh(he);
    zero(nh, H);
    for (h = 0; h < nh; h++) {
        n = nxt(h); nn = nxt(n);
        i = ver(h); j = ver(n); k = ver(nn);
        get3(x, y, z, i, j, k, /**/ a, b, c);
        cot = tri_cot(b, c, a);
        H[h] += cot;
        if (!bnd(h)) H[flp(h)] += cot;
    }
    return HE_OK;
}

static int compute_laplace(He *he, const real *V0, const real *t, const real *area, /**/ real *V1) {
    int h, n, nv, nh, i, j;
    nv = he_nv(he);
    zero(nv, V1);
    nh = he_nh(he);
    for (h = 0; h < nh; h++) {
        n = nxt(h);
        i = ver(h); j = ver(n);
        V1[i] += t[h]*(V0[i] - V0[j])/2;
    }
    for (i = 0; i < nv; i++)
        V1[i] /= area[i];
    return HE_OK;
}

static int compute_area(He *he,
                         const real *x, const real *y, const real *z, /**/
                         real *area) {
    enum {X, Y, Z};
    int t, nt, nv;
    int i, j, k;
    real a[3], b[3], c[3], u[3];
    int *T0, *T1, *T2;
    real area0;
    real theta_a, theta_b, theta_c;
    real cota,cotb,cotc;
    real ab2, bc2, ca2;

    nt = he_nt(he);
    nv = he_nv(he);
    he_T(he, /**/ &T0, &T1, &T2);
    zero(nv, area);

    for ( t = 0; t < nt; t++ ) {
        i = T0[t]; j = T1[t]; k = T2[t];
        get3(x, y, z, i, j, k, a, b, c);
        area0 = tri_area(a, b, c);
        theta_a = tri_angle(c, a, b);
        theta_b = tri_angle(a, b, c);
        theta_c = tri_angle(b, c, a);

        if (theta_a > pi/2.0) {
            area[i] += area0/2;
            area[j] += area0/4;
            area[k] += area0/4;
        }
        else if ( theta_b > pi/2.0 ) {
            area[j] += area0/2;
            area[i] += area0/4;
            area[k] += area0/4;
        }
        else if ( theta_c > pi/2.0 ) {
            area[k] += area0/2;
            area[i] += area0/4;
            area[j] += area0/4;
        } else {
            vec_minus(a, b,  u);
            ab2 = vec_dot(u, u);

            vec_minus(b, c, u);
            bc2 = vec_dot(u, u);

            vec_minus(c, a,  u);
            ca2 = vec_dot(u, u);

            cota = tri_cot(c, a, b);
            cotb = tri_cot(a, b, c);
            cotc = tri_cot(b, c, a);

            area[i] += ( ab2*cotc + ca2*cotb ) / 8;
            area[j] += ( bc2*cota + ab2*cotc ) / 8;
            area[k] += ( ca2*cotb + bc2*cota ) / 8;
        }
    }
    return HE_OK;
}

static int compute_norm(He *he,
                        const real *x, const real *y, const real *z, /**/
                        real *normx, real *normy, real *normz) {
    enum {X, Y, Z};
    int t, nt;
    int i, j, k, nv;
    real a[3], b[3], c[3], u[3], u0[3];
    int *T0, *T1, *T2;
    real theta_a, theta_b, theta_c;

    nt = he_nt(he);
    nv = he_nv(he);
    he_T(he, &T0, &T1, &T2);
    zero(nv, normx); zero(nv, normy); zero(nv, normz);
    for ( t = 0; t < nt; t++ ) {
        i = T0[t]; j = T1[t]; k = T2[t];
        get3(x, y, z, i, j, k, a, b, c);
        theta_a = tri_angle(c, a, b);
        theta_b = tri_angle(a, b, c);
        theta_c = tri_angle(b, c, a);
        tri_normal(a, b, c, u);
        vec_scalar_append(u, theta_a, i, normx, normy, normz);
        vec_scalar_append(u, theta_b, j, normx, normy, normz);
        vec_scalar_append(u, theta_c, k, normx, normy, normz);
    }

    for (i = 0; i < nv; i++) {
        vec_get(i, normx, normy, normz, /**/ u);
        vec_norm(u, /**/ u0);
        vec_set(u0, i, /**/ normx, normy, normz);
    }
    return HE_OK;
}
static int compute_curva_mean(He *he,
                              const real *lbx, const real *lby, const real *lbz,
                              const real *normx, const real *normy, const real *normz,
                              /**/ real *curva_mean) {
    enum {X, Y, Z};
    int i, nv;
    real u[3], v[3];
    nv  = he_nv(he);
    for ( i = 0; i < nv; i++ ) {
        vec_get(i, lbx, lby, lbz, u);
        vec_get(i, normx, normy, normz, v);
        curva_mean[i] = vec_dot(u, v)/2;
    }
    return HE_OK;
}
static int compute_curva_gauss(He *he, const real *area,
                               const real *x, const real *y, const real *z, /**/
                               real *curva_gauss) {

    int *T0, *T1, *T2;
    int t, nt;
    int i, j, k, nv;
    real a[3], b[3], c[3];
    real theta_a, theta_b, theta_c;

    nt = he_nt(he);
    nv = he_nv(he);
    he_T(he, &T0, &T1, &T2);
    zero(nv,  curva_gauss);

    for ( t = 0; t < nt; t++ ) {
        i = T0[t]; j = T1[t]; k = T2[t];

        get3(x, y, z, i, j, k, a, b, c);

        theta_a = tri_angle(c, a, b);
        theta_b = tri_angle(a, b, c);
        theta_c = tri_angle(b, c, a);

        curva_gauss[i] -= theta_a;
        curva_gauss[j] -= theta_b;
        curva_gauss[k] -= theta_c;
    }

    for ( i = 0; i < nv; i++ ) {
        curva_gauss[i] = ( curva_gauss[i] + 2 * pi ) / area[i];
    }

    return HE_OK;
}
real he_f_canham_energy(T *q, He *he,
                        const real *x, const real *y, const real *z) {
    enum {X, Y, Z};
    int v, m;
    int i, j, k;
    int *T0, *T1, *T2;
    real *lbx, *lby, *lbz;
    real *normx, *normy, *normz;
    real *curva_mean;
    real *energy, *area, *t;

    real Kb;
    real area_tot_tri;
    int  nv, nt;

    Kb   = q->Kb;

    nv = he_nv(he);
    nt = he_nt(he);

    if (nv != q->nv )
        ERR(HE_INDEX, "he_nv(he)=%d != nv = %d", nv, q->nv);
    if (nt != q->nt )
        ERR(HE_INDEX, "he_nt(he)=%d != nt = %d", nt, q->nt);

    he_T(he, &T0, &T1, &T2);
    lbx = q->lbx; lby = q->lby; lbz = q->lbz;
    normx = q->normx; normy = q->normy; normz = q->normz;
    curva_mean  = q->curva_mean;
    energy  = q->energy;
    area    = q->area;
    t = q->t;

    compute_area(he, x, y, z, area);

    compute_cot(he, x, y, z, /**/ t);
    compute_laplace(he, x, t, area, /**/ lbx);
    compute_laplace(he, y, t, area, /**/ lby);
    compute_laplace(he, z, t, area, /**/ lbz);

    compute_norm(he, x, y, z, normx, normy, normz);
    compute_curva_mean(he, lbx, lby, lbz, normx, normy, normz, /**/ curva_mean);

    for (v = 0; v < nv; v++)
        energy[v] = curva_mean[v]*curva_mean[v]*area[v];

    scale(nv, 2*Kb, energy);

    return sum(nv, energy);

}
int he_f_canham_force(T *q, He *he,
                      const real *x, const real *y, const real *z, /**/
                      real *fx_tot, real *fy_tot, real *fz_tot) {
    enum {X, Y, Z};
    int v, e, m;
    int i, j, k, l;
    int nv, nt, ne;
    int *T0, *T1, *T2;
    real *fx, *fy, *fz;
    real *lbx, *lby, *lbz, *t;
    real *normx, *normy, *normz;
    real *area;
    real *curva_gauss, *curva_mean, *lpl;
    real fm;

    real Kb;

    Kb   = q->Kb;

    nv = he_nv(he);
    nt = he_nt(he);
    ne = he_ne(he);

    he_T(he, &T0, &T1, &T2);
    lbx = q->lbx; lby = q->lby; lbz = q->lbz;
    fx = q->fx; fy = q->fy; fz = q->fz;

    normx = q->normx; normy = q->normy; normz = q->normz;
    curva_mean  = q->curva_mean;
    curva_gauss = q->curva_gauss;
    area    = q->area;
    t = q->t; lpl = q->lpl;

    zero(nv, fx); zero(nv, fy); zero(nv, fz);
    compute_area(he, x, y, z, area);
    compute_cot(he, x, y, z, /**/ t);
    compute_laplace(he, x, t, area, /**/ lbx);
    compute_laplace(he, y, t, area, /**/ lby);
    compute_laplace(he, z, t, area, /**/ lbz);

    compute_norm(he, x, y, z, normx, normy, normz);
    compute_curva_mean(he, lbx, lby, lbz, normx, normy, normz, /**/ curva_mean);
    compute_curva_gauss(he, area, x, y, z, curva_gauss);
    compute_laplace(he, curva_mean, t, area, /**/ lpl);
    
    for (v = 0; v < nv; v++) {
        fm = 2*(curva_mean[v])*(curva_mean[v]*curva_mean[v]-curva_gauss[v]);
        fm += lpl[v];
        fm *= area[v];
        fx[v] += fm * normx[v];
        fy[v] += fm * normy[v];
        fz[v] += fm * normz[v];
    }

    scale(nv, 2*Kb, fx);
    scale(nv, 2*Kb, fy);
    scale(nv, 2*Kb, fz);

    plus(nv, fx, fx_tot);
    plus(nv, fy, fy_tot);
    plus(nv, fz, fz_tot);

    return HE_OK;
}
