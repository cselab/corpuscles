#include <stdio.h>

#include "real.h"
#include "he/memory.h"
#include "he/err.h"
#include "he/he.h"
#include "he/vec.h"
#include "he/tri.h"
#include "he/dtri.h"
#include "he/sum.h"
#include "he/macro.h"

#include "he/f/gompper_xin.h"

#define T HeFGompperXin

#define  nxt(h)     he_nxt(he, (h))
#define  flp(h)     he_flp(he, (h))
#define  ver(h)     he_ver(he, (h))
#define  hdg_ver(v) he_hdg_ver(he, (v))
#define  hdg_edg(e) he_hdg_edg(he, (e))
#define  hdg_tri(e) he_hdg_tri(he, (e))
#define  bnd(h)     he_bnd(he, (h))

struct T {
    real Kb;

    int *T0, *T1, *T2;
    real *l2, *t;

    real *lbx, *lby, *lbz;
    real *normx, *normy, *normz;
    real *H, *curva_gauss;
    real *energy, *area;

    int nv, ne, nt;
};
static void get_edg(int i, int j, const real *x, const real *y, const real *z, /**/ real r[3]) {
    real a[3], b[3];
    vec_get(i, x, y, z, a);
    vec_get(j, x, y, z, b);
    vec_minus(a, b, r);
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
static void zero(int n, real *a) {
    int i;
    for (i = 0; i < n; i++) a[i] = 0;
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

static int scale(int n, real sc, /*io*/ real *a) {
    int i;
    for (i = 0; i < n; i++)
        a[i] *= sc;
    return HE_OK;
}

int he_f_gompper_xin_ini(real Kb, __UNUSED real C0, __UNUSED real Kad, __UNUSED real DA0D, He *he, T **pq) {
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

    MALLOC(nh, &q->l2);
    MALLOC(nh, &q->t);

    MALLOC(nt, &q->T0); MALLOC(nt, &q->T1); MALLOC(nt, &q->T2);

    MALLOC(nv, &q->lbx); MALLOC(nv, &q->lby); MALLOC(nv, &q->lbz);
    MALLOC(nv, &q->normx); MALLOC(nv, &q->normy); MALLOC(nv, &q->normz);
    MALLOC(nv, &q->H);  MALLOC(nv, &q->curva_gauss);

    MALLOC(nv, &q->energy);
    MALLOC(nv, &q->area);

    *pq = q;
    return HE_OK;
}
int he_f_gompper_xin_fin(T *q) {
    FREE(q->T0); FREE(q->T1); FREE(q->T2);
    FREE(q->t); FREE(q->l2);
    FREE(q->lbx); FREE(q->lby); FREE(q->lbz);
    FREE(q->normx);FREE(q->normy);FREE(q->normz);
    FREE(q->H);FREE(q->curva_gauss);
    FREE(q->energy); FREE(q->area);
    FREE(q);
    return HE_OK;
}
int he_f_gompper_xin_area_ver(T *q, /**/ real **pa) {
    *pa = q->area;
    return HE_OK;
}
int he_f_gompper_xin_lb_ver(T *q, /**/ real **px, real **py, real **pz ) {
    *px = q->lbx;
    *py = q->lby;
    *pz = q->lbz;
    return HE_OK;
}
int he_f_gompper_xin_norm_ver(T *q, /**/ real **px, real **py, real **pz ) {
    *px = q->normx;
    *py = q->normy;
    *pz = q->normz;
    return HE_OK;
}
int he_f_gompper_xin_H_ver(T *q, /**/ real **pa) {
    *pa = q->H;
    return HE_OK;
}
int he_f_gompper_xin_curva_gauss_ver(T *q, /**/ real **pa) {
    *pa = q->curva_gauss;
    return HE_OK;
}
int he_f_gompper_xin_energy_ver(T *q, /**/ real**pa) {
    *pa = q->energy;
    return HE_OK;
}
static void compute_l2(He *he, const real *x, const real *y, const real *z, /**/ real *l2) {
    int nh;
    int h, n;
    int i, j;
    real r[3];
    nh = he_nh(he);
    for (h = 0; h < nh; h++) {
        n = nxt(h);
        i = ver(h); j = ver(n);
        get_edg(i, j, x, y, z, /**/ r);
        l2[h] = vec_dot(r, r);
    }
}
static void compute_cot(He *he, const real *x, const real *y, const real *z, /**/ real *H) {
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
}
static void compute_area(He *he, const real *l2, const real *t, /**/ real *V) {
    int nv, nh, h, i;
    nv = he_nv(he);
    zero(nv, V);
    nh = he_nh(he);
    for (h = 0; h < nh; h++) {
        i = ver(h);
        V[i] += t[h]*l2[h]/8;
    }
}
static void compute_lb(He *he, const real *V0, const real *t, const real *area, /**/ real *V1) {
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
}
static int compute_norm(He *he,
                        const real *x, const real *y, const real *z, /**/
                        real *normx, real *normy, real *normz) {
    enum {X, Y, Z};
    int t, nt, h, n, nn;
    int i, j, k, nv;
    real a[3], b[3], c[3], u[3], u0[3];
    real theta_a, theta_b, theta_c;

    nt = he_nt(he);
    nv = he_nv(he);

    zero(nv, normx); zero(nv, normy); zero(nv, normz);
    for ( t = 0; t < nt; t++ ) {
        h = hdg_tri(t);
        n = nxt(h); nn = nxt(n);
        i = ver(h); j = ver(n); k = ver(nn);        
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
static int compute_H(He *he,
                     real *lbx, real *lby, real *lbz,
                     real *normx, real *normy, real *normz,
                     /**/ real *H) {
    enum {X, Y, Z};
    int i, nv;
    real u[3], v[3];

    nv  = he_nv(he);

    for ( i = 0; i < nv; i++ ) {
        vec_get(i, lbx, lby, lbz, u);
        vec_get(i, normx, normy, normz, v);
        H[i] = vec_dot(u, v)/2;
    }

    return HE_OK;

}
static int compute_energy(int nv, const real *H, const real *area, /**/ real *energy) {
    int i;
    for (i = 0; i < nv; i++)
        energy[i]   = H[i]*H[i]*area[i];
    return HE_OK;
}

real he_f_gompper_xin_energy(T *q, He *he,
                             const real *x, const real *y, const real *z) {
    int nv, nt;
    int i, j, k, l;
    int *T0, *T1, *T2;

    real Kb;
    real *l2, *t;
    real *lbx, *lby, *lbz;
    real *normx, *normy, *normz;
    real *H;
    real *energy, *area;

    T0 = q->T0; T1 = q->T1; T2 = q->T2;
    l2 = q->l2; t = q->t;
    lbx = q->lbx; lby = q->lby; lbz = q->lbz;
    normx = q->normx; normy = q->normy; normz = q->normz;
    H  = q->H;
    area = q->area; energy = q->energy;
    Kb = q->Kb;

    nv = q->nv;
    nt = he_nt(he);

    for (l = 0; l < nt; l++) {
        get_ijk(l, he, /**/ &i, &j, &k);
        T0[l] = i; T1[l] = j; T2[l] = k;
    }

    if (he_nv(he) != nv)
        ERR(HE_INDEX, "he_nv(he)=%d != nv = %d", he_nv(he), nv);

    compute_l2(he, x, y, z, /**/ l2);
    compute_cot(he, x, y, z, /**/ t);
    compute_area(he, l2, t, /**/ area);
    compute_lb(he, x, t, area, /**/ lbx);
    compute_lb(he, y, t, area, /**/ lby);
    compute_lb(he, z, t, area, /**/ lbz);
    compute_norm(he, x, y, z, normx, normy, normz);
    compute_H(he, lbx, lby, lbz, normx, normy, normz, /**/ H);
    compute_energy(nv, H, area, /**/ energy);
    scale(nv, 2*Kb, /**/ energy);

    return sum(nv, energy);
}
static void compute_force_t(T *q, He *he,
                            const real *x, const real *y, const real *z,
                            const real *t,
                            const real *lbx, const real *lby, const real *lbz,
                            /**/ real *fx, real *fy, real *fz) {
    real Kb;
    int nh;
    int h, n;
    int i, j;
    real r[3], ll[3], df[3];
    real t0, l2;

    Kb   = q->Kb;

    nh = he_nh(he);

    for (h = 0; h < nh; h++) {
        n = nxt(h); i = ver(h); j = ver(n);
        t0 = t[h];
        get_edg(i, j, x, y, z, /**/  r);
        vec_get(i, lbx, lby, lbz, ll);
        l2 = vec_dot(ll, ll);
        vec_linear_combination(Kb*t0/2, ll, -Kb*t0*l2/8, r,  df);
        vec_append(df, i, /**/ fx, fy, fz);
        vec_substr(df, j, /**/ fx, fy, fz);
    }

}
static void compute_force_dt(T *q, He *he,
                             const real *x, const real *y, const real *z,
                             const real *lbx, const real *lby, const real *lbz,
                             /**/ real *fx, real *fy, real *fz) {
    real Kb;
    int nh;
    int h, n, nn;
    int i, j, k;
    real r[3], a[3], b[3], c[3], da[3], db[3], dc[3];
    real li[3], lk[3];
    real dl, dd, r2, C;

    Kb   = q->Kb;

    nh = he_nh(he);

    for (h = 0; h < nh; h++) {
        n = nxt(h); nn = nxt(n);
        i = ver(h); j = ver(n); k = ver(nn);
        get3(x, y, z, i, j, k,  a, b, c);
        dtri_cot(a, b, c, /**/ da, db, dc);
        get_edg(i, k, x, y, z, /**/ r);
        vec_get(k, lbx, lby, lbz, /**/ lk);
        vec_get(i, lbx, lby, lbz, /**/ li);
        r2 = vec_dot(r, r);
        dl = vec_dot(lk, lk) + vec_dot(li, li);
        dd = vec_dot(li, r)  - vec_dot(lk, r);
        C = Kb*dd/2 - Kb*r2*dl/16;
        vec_scalar_append(da,  C,  i, /**/ fx, fy, fz);
        vec_scalar_append(db,  C,  j, /**/ fx, fy, fz);
        vec_scalar_append(dc,  C,  k, /**/ fx, fy, fz);
    }

}
int he_f_gompper_xin_force(T *q, He *he,
                           const real *x, const real *y, const real *z, /**/
                           real *fx, real *fy, real *fz) {
    int nv, nt;
    int i, j, k, l;
    int *T0, *T1, *T2;

    real *l2, *t;
    real *lbx, *lby, *lbz;
    real *area;


    T0 = q->T0; T1 = q->T1; T2 = q->T2;
    area = q->area;
    l2 = q->l2; t = q->t;
    lbx = q->lbx; lby = q->lby; lbz = q->lbz;

    nv = q->nv;

    nt = he_nt(he);
    for (l = 0; l < nt; l++) {
        get_ijk(l, he, /**/ &i, &j, &k);
        T0[l] = i; T1[l] = j; T2[l] = k;
    }

    if (he_nv(he) != nv)
        ERR(HE_INDEX, "he_nv(he)=%d != nv = %d", he_nv(he), nv);

    compute_l2(he, x, y, z, /**/ l2);
    compute_cot(he, x, y, z, /**/ t);
    compute_area(he, l2, t, /**/ area);
    compute_lb(he, x, t, area, /**/ lbx);
    compute_lb(he, y, t, area, /**/ lby);
    compute_lb(he, z, t, area, /**/ lbz);

    compute_force_t(q, he, x, y, z, t, lbx, lby, lbz,
                    /**/ fx, fy, fz);
    compute_force_dt(q, he, x, y, z, lbx, lby, lbz,
                     /**/ fx, fy, fz);

    return HE_OK;
}
