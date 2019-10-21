#include <stdio.h>
#include <tgmath.h>
#include <stdlib.h>

#include "real.h"
#include "co/ddih.h"
#include "co/dih.h"
#include "co/err.h"
#include "co/he.h"
#include "co/macro.h"
#include "co/memory.h"
#include "co/normal.h"
#include "co/tri.h"
#include "co/vec.h"


#include "co/f/meyer.h"

#define T HeFMeyer

#define  nxt(h)     he_nxt(he, h)
#define  flp(h)     he_flp(he, h)
#define  ver(h)     he_ver(he, h)
#define  hdg_ver(v) he_hdg_ver(he, v)
#define  hdg_edg(e) he_hdg_edg(he, e)
#define  hdg_tri(t) he_hdg_tri(he, t)
#define  bnd(h)     he_bnd(he, h)

static const real pi = 3.141592653589793115997964;

struct T {
    real Kb;
    real *cot;
    real *lbx, *lby, *lbz;
    real *normx, *normy, *normz;
    real *H, *K;
    real *energy_local, *area;
    real *lbH;
    real *f, *fa, *fb, *fc;
    real eng_bend, eng_ad;

    int (*compute_area)(He *, const real *, const real *, const real *,
                        real * area);
    int (*compute_norm)(T *, He *, const real *, const real *,
                        const real *, /**/ real *, real *, real *);
    int (*compute_H)(T *, He *, /**/ real *);
};

static int
zero(int n, real * a)
{
    int i;

    for (i = 0; i < n; i++)
        a[i] = 0;
    return CO_OK;
}

static int
get3(const real * x, const real * y, const real * z,
     int i, int j, int k, /**/ real a[3], real b[3], real c[3])
{
    vec_get(i, x, y, z, /**/ a);
    vec_get(j, x, y, z, /**/ b);
    vec_get(k, x, y, z, /**/ c);
    return CO_OK;
}

static int
norm_mwn(__UNUSED T * q, He * he,
         const real * x, const real * y, const real * z, /**/
         real * normx, real * normy, real * normz)
{
    int status, i, n;

    n = he_nv(he);
    status = normal_mwa(he, x, y, z, normx, normy, normz);
    for (i = 0; i < n; i++) {
        normx[i] = -normx[i];
        normy[i] = -normy[i];
        normz[i] = -normz[i];
    }
    return status;
}

static int
norm_lap(T * q, He * he,
         const real * x, const real * y, const real * z, /**/
         real * nx, real * ny, real * nz)
{
    int nv, i, status;
    const real *lbx, *lby, *lbz;
    real lb[3], n[3], u[3];

    lbx = q->lbx;
    lby = q->lby;
    lbz = q->lbz;
    status = normal_mwa(he, x, y, z, nx, ny, nz);

    nv = he_nv(he);
    for (i = 0; i < nv; i++) {
        vec_get(i, lbx, lby, lbz, lb);
        vec_get(i, nx, ny, nz, n);
        vec_norm(lb, u);

        if (vec_dot(n, lb) < 0)
            vec_neg(u);

        vec_set(u, i, nx, ny, nz);
    }
    return status;
}

static int
area_voronoi(He * he,
             const real * x, const real * y, const real * z, /**/
             real * area)
{
    enum { X, Y, Z };
    int t, nt, nv;
    int i, j, k;
    real a[3], b[3], c[3], u[3];
    int *T0, *T1, *T2;
    real cota, cotb, cotc;
    real ab2, bc2, ca2;

    nt = he_nt(he);
    nv = he_nv(he);

    he_T(he, &T0, &T1, &T2);
    zero(nv, area);

    for (t = 0; t < nt; t++) {
        i = T0[t];
        j = T1[t];
        k = T2[t];

        get3(x, y, z, i, j, k, a, b, c);

        vec_minus(a, b, u);
        ab2 = vec_dot(u, u);

        vec_minus(b, c, u);
        bc2 = vec_dot(u, u);

        vec_minus(c, a, u);
        ca2 = vec_dot(u, u);

        cota = tri_cot(c, a, b);
        cotb = tri_cot(a, b, c);
        cotc = tri_cot(b, c, a);

        area[i] += (ab2 * cotc + ca2 * cotb) / 8;
        area[j] += (bc2 * cota + ab2 * cotc) / 8;
        area[k] += (ca2 * cotb + bc2 * cota) / 8;

    }                           /*end for loop */
    return CO_OK;
}

static int
area_mix(He * he,
         const real * x, const real * y, const real * z, /**/ real * area)
{
    enum { X, Y, Z };
    int t, nt, nv;
    int i, j, k;
    real a[3], b[3], c[3], u[3];
    int *T0, *T1, *T2;
    real area0;
    real theta_a, theta_b, theta_c;
    real cota, cotb, cotc;
    real ab2, bc2, ca2;

    nt = he_nt(he);
    nv = he_nv(he);
    he_T(he, &T0, &T1, &T2);

    zero(nv, area);

    for (t = 0; t < nt; t++) {
        i = T0[t];
        j = T1[t];
        k = T2[t];

        get3(x, y, z, i, j, k, a, b, c);
        area0 = tri_area(a, b, c);

        theta_a = tri_angle(c, a, b);
        theta_b = tri_angle(a, b, c);
        theta_c = tri_angle(b, c, a);

        if (theta_a > pi / 2.0) {
            area[i] += area0 / 2;
            area[j] += area0 / 4;
            area[k] += area0 / 4;
        } else if (theta_b > pi / 2.0) {
            area[j] += area0 / 2;
            area[i] += area0 / 4;
            area[k] += area0 / 4;
        } else if (theta_c > pi / 2.0) {
            area[k] += area0 / 2;
            area[i] += area0 / 4;
            area[j] += area0 / 4;
        } else {
            vec_minus(a, b, u);
            ab2 = vec_dot(u, u);

            vec_minus(b, c, u);
            bc2 = vec_dot(u, u);

            vec_minus(c, a, u);
            ca2 = vec_dot(u, u);

            cota = tri_cot(c, a, b);
            cotb = tri_cot(a, b, c);
            cotc = tri_cot(b, c, a);

            area[i] += (ab2 * cotc + ca2 * cotb) / 8;
            area[j] += (bc2 * cota + ab2 * cotc) / 8;
            area[k] += (ca2 * cotb + bc2 * cota) / 8;
        }

    }                           /*end for loop */
    return CO_OK;
}

static int
H_norm(T * q, He * he, /**/ real * H)
{
    enum { X, Y, Z };
    int i, nv;
    real *lbx, *lby, *lbz;
    real *normx, *normy, *normz;
    real u[3], v[3];

    nv = he_nv(he);
    lbx = q->lbx;
    lby = q->lby;
    lbz = q->lbz;

    normx = q->normx;
    normy = q->normy;
    normz = q->normz;
    for (i = 0; i < nv; i++) {
        vec_get(i, lbx, lby, lbz, u);
        vec_get(i, normx, normy, normz, v);
        H[i] = vec_dot(u, v) / 2;
    }
    return CO_OK;
}

static int
H_lap(T * q, He * he, /**/ real * H)
{
    enum { X, Y, Z };
    int i, nv;
    real *lbx, *lby, *lbz, *nx, *ny, *nz;
    real lb[3], n[3];

    nv = he_nv(he);
    lbx = q->lbx;
    lby = q->lby;
    lbz = q->lbz;
    nx = q->normx;
    ny = q->normy;
    nz = q->normz;

    for (i = 0; i < nv; i++) {
        vec_get(i, nx, ny, nz, n);
        vec_get(i, lbx, lby, lbz, lb);
        H[i] = vec_abs(lb) / 2;
        if (vec_dot(n, lb) < 0)
            H[i] = -H[i];
    }
    return CO_OK;
}


int
he_f_meyer_ini(real Kb, He * he, T ** pq)
{
#define M(n, f) MALLOC(n, &q->f)
#define S(f) q->f = f
    T *q;
    int nv, nh;

    MALLOC(1, &q);
    nv = he_nv(he);
    nh = he_nh(he);
    q->Kb = Kb;
    if (getenv("MIX"))
        q->compute_area = area_mix;
    else
        q->compute_area = area_voronoi;

    if (getenv("LAP")) {
        MSG("LAP");
        q->compute_norm = norm_lap;
        q->compute_H = H_lap;
    } else {
        q->compute_norm = norm_mwn;
        q->compute_H = H_norm;
    }

    M(nh, cot);
    M(nv, lbx);
    M(nv, lby);
    M(nv, lbz);
    M(nv, normx);
    M(nv, normy);
    M(nv, normz);
    M(nv, H);
    M(nv, K);
    M(nv, area);
    M(nv, energy_local);
    M(nv, lbH);
    M(nv, f);
    M(nv, fa);
    M(nv, fb);
    M(nv, fc);

    *pq = q;
    return CO_OK;
#undef M
#undef S
}

int
he_f_meyer_argv(__UNUSED char ***p, __UNUSED He * he, __UNUSED T ** pq)
{
    return CO_OK;
}

int
he_f_meyer_fin(T * q)
{
#define F(x) FREE(q->x)
    F(cot);
    F(lbx);
    F(lby);
    F(lbz);
    F(normx);
    F(normy);
    F(normz);
    F(H);
    F(K);
    F(area);
    F(energy_local);
    F(lbH);
    F(f);
    F(fa);
    F(fb);
    F(fc);
    FREE(q);
    return CO_OK;
#undef F
}

int
he_f_meyer_area_ver(T * q, /**/ real ** pa)
{
    *pa = q->area;
    return CO_OK;
}

int
he_f_meyer_laplace_ver(T * q, /**/ real ** px, real ** py, real ** pz)
{
    *px = q->lbx;
    *py = q->lby;
    *pz = q->lbz;
    return CO_OK;
}

int
he_f_meyer_norm_ver(T * q, /**/ real ** px, real ** py, real ** pz)
{
    *px = q->normx;
    *py = q->normy;
    *pz = q->normz;
    return CO_OK;
}

int
he_f_meyer_curva_mean_ver(T * q, /**/ real ** pa)
{
    *pa = q->H;
    return CO_OK;
}

int
he_f_meyer_K_ver(T * q, /**/ real ** pa)
{
    *pa = q->K;
    return CO_OK;
}

int
he_f_meyer_energy_ver(T * q, /**/ real ** pa)
{
    *pa = q->energy_local;
    return CO_OK;
}

int
he_f_meyer_laplace_H_ver(T * q, /**/ real ** px)
{
    *px = q->lbH;
    return CO_OK;
}

int
he_f_meyer_components(T * q, /**/ real ** f, real ** fa, real ** fb,
                      real ** fc)
{
    *f = q->f;
    *fa = q->fa;
    *fb = q->fb;
    *fc = q->fc;
    return CO_OK;
}

static int
compute_cot(He * he, const real * x, const real * y, const real * z,
            /**/ real * cot)
{
    int nh, h, n, nn;
    int i, j, k;
    real a[3], b[3], c[3], tt;

    nh = he_nh(he);
    zero(nh, cot);
    for (h = 0; h < nh; h++) {
        n = nxt(h);
        nn = nxt(n);
        i = ver(h);
        j = ver(n);
        k = ver(nn);
        get3(x, y, z, i, j, k, /**/ a, b, c);
        tt = tri_cot(b, c, a);
        cot[h] += tt;
        if (!bnd(h))
            cot[flp(h)] += tt;
    }
    return CO_OK;
}

static int
compute_lb(T * q, He * he, const real * x, /**/ real * lbx)
{
    enum { X, Y, Z };
    int i, j, h, n, nv, nh;
    real *area, *cot;

    nh = he_nh(he);
    nv = he_nv(he);

    area = q->area;
    cot = q->cot;

    zero(nv, lbx);

    for (h = 0; h < nh; h++) {
        n = nxt(h);
        i = ver(h);
        j = ver(n);
        lbx[i] -= cot[h] * (x[i] - x[j]) / 2;
    }

    for (i = 0; i < nv; i++) {
        lbx[i] /= area[i];
    }

    return CO_OK;

}

static int
compute_K(T * q, He * he,
          const real * x, const real * y, const real * z, /**/ real * K)
{
    real *area;
    int t, nt;
    int i, j, k, nv;
    real a[3], b[3], c[3];
    real theta_a, theta_b, theta_c;
    int *T0, *T1, *T2;

    nt = he_nt(he);
    nv = he_nv(he);
    he_T(he, &T0, &T1, &T2);

    area = q->area;
    zero(nv, K);

    for (t = 0; t < nt; t++) {
        i = T0[t];
        j = T1[t];
        k = T2[t];

        get3(x, y, z, i, j, k, a, b, c);

        theta_a = tri_angle(c, a, b);
        theta_b = tri_angle(a, b, c);
        theta_c = tri_angle(b, c, a);

        K[i] -= theta_a;
        K[j] -= theta_b;
        K[k] -= theta_c;
    }

    for (i = 0; i < nv; i++) {
        K[i] = (K[i] + 2 * pi) / area[i];
    }

    return CO_OK;
}

real
he_f_meyer_energy(T * q, He * he,
                  const real * x, const real * y, const real * z)
{
    enum { X, Y, Z };
    int v;
    int *T0, *T1, *T2;
    real *lbx, *lby, *lbz;
    real *normx, *normy, *normz;
    real *H;
    real *energy_local, *area, *cot;

    real Kb;
    int nv;

    real mH1, mH2;

    Kb = q->Kb;
    nv = he_nv(he);

    he_T(he, &T0, &T1, &T2);
    lbx = q->lbx;
    lby = q->lby;
    lbz = q->lbz;
    normx = q->normx;
    normy = q->normy;
    normz = q->normz;
    H = q->H;
    energy_local = q->energy_local;
    area = q->area;
    cot = q->cot;

    q->compute_area(he, x, y, z, area);

    compute_cot(he, x, y, z, cot);
    compute_lb(q, he, x, lbx);
    compute_lb(q, he, y, lby);
    compute_lb(q, he, z, lbz);
    q->compute_norm(q, he, x, y, z, normx, normy, normz);
    q->compute_H(q, he, /**/ H);

    mH1 = 0;
    mH2 = 0;

    for (v = 0; v < nv; v++) {
        mH1 += H[v] * area[v];
        mH2 += H[v] * H[v] * area[v];
        energy_local[v] = 2 * Kb * (H[v]) * (H[v]) * area[v];
    }

    return 2 * Kb * mH2;
}

int
he_f_meyer_force(T * q, He * he,
                 const real * x, const real * y, const real * z, /**/
                 real * fx, real * fy, real * fz)
{
#define G(f) f = q->f
    enum { X, Y, Z };
    int v;
    int nv;
    int *T0, *T1, *T2;
    int *D0, *D1, *D2, *D3;
    real *lbx, *lby, *lbz;
    real *normx, *normy, *normz;
    real *area, *cot;
    real *K, *H;
    real *lbH, *f, *fa, *fb, *fc;
    real fm;

    real Kb;

    Kb = q->Kb;

    nv = he_nv(he);

    he_T(he, &T0, &T1, &T2);
    he_D(he, &D0, &D1, &D2, &D3);

    G(cot);
    G(lbx);
    G(lby);
    G(lbz);
    G(normx);
    G(normy);
    G(normz);
    G(H);
    G(K);
    G(area);
    G(lbH);
    G(f);
    G(fa);
    G(fb);
    G(fc);
    q->compute_area(he, x, y, z, area);

    compute_cot(he, x, y, z, cot);
    compute_lb(q, he, x, lbx);
    compute_lb(q, he, y, lby);
    compute_lb(q, he, z, lbz);
    q->compute_norm(q, he, x, y, z, normx, normy, normz);
    q->compute_H(q, he, H);
    compute_K(q, he, x, y, z, K);
    compute_lb(q, he, H, lbH);

    for (v = 0; v < nv; v++) {
        fm = lbH[v] + 2 * H[v] * (H[v] * H[v] - K[v]);
        fm *= area[v];
        fm *= 2 * Kb;
        fx[v] += fm * normx[v];
        fy[v] += fm * normy[v];
        fz[v] += fm * normz[v];

        fa[v] = fabs(lbH[v]);
        fb[v] = fabs(2 * H[v] * K[v]);
        fc[v] = fabs(H[v] * H[v] * H[v]);
        f[v] = fabs(lbH[v] + 2 * H[v] * (H[v] * H[v] - K[v]));
    }

    return CO_OK;
#undef G
}

real
he_f_meyer_energy_ad(T * q)
{
    return q->eng_ad;
}

real
he_f_meyer_energy_bend(T * q)
{
    return q->eng_bend;
}
