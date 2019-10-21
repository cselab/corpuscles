#include <stdio.h>
#include <tgmath.h>

#include "real.h"
#include "co/macro.h"
#include "co/memory.h"
#include "co/err.h"
#include "co/he.h"
#include "co/vec.h"
#include "co/tri.h"
#include "co/dtri.h"

#include "co/f/gompper_kroll.h"

#define T HeFGompperKroll

#define  nxt(h)     he_nxt(he, (h))
#define  flp(h)     he_flp(he, (h))
#define  ver(h)     he_ver(he, (h))
#define  hdg_ver(v) he_hdg_ver(he, (v))
#define  hdg_edg(e) he_hdg_edg(he, (e))
#define  hdg_tri(e) he_hdg_tri(he, (e))
#define  bnd(h)     he_bnd(he, (h))

static const real pi = 3.141592653589793115997964;
static const real epsilon = 1.0e-12;

struct T {
    int nv, ne, nt, nh;
    real Kb, C0, Kad, DA0D;
    int *T0, *T1, *T2;
    real *len2, *cot;
    real *lbx, *lby, *lbz;
    real *normx, *normy, *normz;
    real *curva_mean, *curva_gauss;
    real *energy_local, *area;
    real eng_bend, eng_ad;
};
static void
get_edg(const real * x, const real * y, const real * z,
        int i, int j, /**/ real r[3])
{
    real a[3], b[3];

    vec_get(i, x, y, z, a);
    vec_get(j, x, y, z, b);
    vec_minus(a, b, r);
}

static int
get_ijk(int t, He * he, /**/ int *pi, int *pj, int *pk)
{
    int h, n, nn, i, j, k;

    h = hdg_tri(t);
    n = nxt(h);
    nn = nxt(n);
    i = ver(h);
    j = ver(n);
    k = ver(nn);
    *pi = i;
    *pj = j;
    *pk = k;
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

void
get4(const real * x, const real * y, const real * z,
     int i, int j, int k, int l, /**/
     real a[3], real b[3], real c[3], real d[3])
{

    /*Given four indices i, j, k l of vertices,
       return their positions in a, b, c, d. */
    vec_get(i, x, y, z, a);
    vec_get(j, x, y, z, b);
    vec_get(k, x, y, z, c);
    vec_get(l, x, y, z, d);
}

static void
zero(int n, real * a)
{
    int i;

    for (i = 0; i < n; i++)
        a[i] = 0;
}

int
he_f_gompper_kroll_ini(real Kb, real C0, real Kad, real DA0D, He * he,
                       T ** pq)
{
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
    q->nh = nh;

    q->Kb = Kb;
    q->C0 = C0;
    q->Kad = Kad;
    q->DA0D = DA0D;

    MALLOC(nt, &q->T0);
    MALLOC(nt, &q->T1);
    MALLOC(nt, &q->T2);

    MALLOC(nh, &q->len2);
    MALLOC(nh, &q->cot);

    MALLOC(nv, &q->lbx);
    MALLOC(nv, &q->lby);
    MALLOC(nv, &q->lbz);
    MALLOC(nv, &q->normx);
    MALLOC(nv, &q->normy);
    MALLOC(nv, &q->normz);
    MALLOC(nv, &q->curva_mean);
    MALLOC(nv, &q->curva_gauss);

    MALLOC(nv, &q->energy_local);
    MALLOC(nv, &q->area);

    *pq = q;
    return CO_OK;
}

int
he_f_gompper_kroll_argv(__UNUSED char ***p, __UNUSED He * he,
                        __UNUSED T ** pq)
{
    return CO_OK;
}

int
he_f_gompper_kroll_fin(T * q)
{
    FREE(q->T0);
    FREE(q->T1);
    FREE(q->T2);
    FREE(q->len2);
    FREE(q->cot);
    FREE(q->lbx);
    FREE(q->lby);
    FREE(q->lbz);
    FREE(q->normx);
    FREE(q->normy);
    FREE(q->normz);
    FREE(q->curva_mean);
    FREE(q->curva_gauss);
    FREE(q->energy_local);
    FREE(q->area);
    FREE(q);
    return CO_OK;
}

int
he_f_gompper_kroll_area_ver(T * q, /**/ real ** pa)
{
    *pa = q->area;
    return CO_OK;
}

int
he_f_gompper_kroll_laplace_ver(T * q, /**/ real ** px, real ** py,
                               real ** pz)
{
    *px = q->lbx;
    *py = q->lby;
    *pz = q->lbz;
    return CO_OK;
}

int
he_f_gompper_kroll_norm_ver(T * q, /**/ real ** px, real ** py, real ** pz)
{
    *px = q->normx;
    *py = q->normy;
    *pz = q->normz;
    return CO_OK;
}

int
he_f_gompper_kroll_curva_mean_ver(T * q, /**/ real ** pa)
{
    *pa = q->curva_mean;
    return CO_OK;
}

int
he_f_gompper_kroll_curva_gauss_ver(T * q, /**/ real ** pa)
{
    *pa = q->curva_gauss;
    return CO_OK;
}

int
he_f_gompper_kroll_energy_ver(T * q, /**/ real ** pa)
{
    *pa = q->energy_local;
    return CO_OK;
}

static void
compute_len2(He * he, const real * x, const real * y, const real * z,
             /**/ real * H)
{
    int nh;
    int h, n;
    int i, j;
    real r[3];

    nh = he_nh(he);
    for (h = 0; h < nh; h++) {
        n = nxt(h);
        i = ver(h);
        j = ver(n);
        get_edg(x, y, z, i, j, /**/ r);
        H[h] = vec_dot(r, r);
    }
}

static void
compute_cot(He * he, const real * x, const real * y, const real * z,
            /**/ real * H)
{
    int nh, h, n, nn;
    int i, j, k;
    real a[3], b[3], c[3], cot;

    nh = he_nh(he);
    zero(nh, H);
    for (h = 0; h < nh; h++) {
        n = nxt(h);
        nn = nxt(n);
        i = ver(h);
        j = ver(n);
        k = ver(nn);
        get3(x, y, z, i, j, k, /**/ a, b, c);
        cot = tri_cot(b, c, a);
        H[h] += cot;
        if (!bnd(h))
            H[flp(h)] += cot;
    }
}

static real
compute_area_voronoi(He * he, const real * len2, const real * cot,
                     /**/ real * V)
{
    int nv, nh, h, i;
    real area0, area_tot;

    nv = he_nv(he);
    zero(nv, V);
    area_tot = 0;
    nh = he_nh(he);
    for (h = 0; h < nh; h++) {
        i = ver(h);
        area0 = cot[h] * len2[h] / 8;
        V[i] += area0;
        area_tot += area0;
    }

    return area_tot;
}

static void
compute_laplace(He * he, const real * V0, const real * cot,
                const real * area, /**/ real * V1)
{
    int h, n, nv, nh, i, j;

    nv = he_nv(he);
    zero(nv, V1);
    nh = he_nh(he);
    for (h = 0; h < nh; h++) {
        n = nxt(h);
        i = ver(h);
        j = ver(n);
        V1[i] -= cot[h] * (V0[i] - V0[j]) / 2;
    }
    for (i = 0; i < nv; i++)
        V1[i] /= area[i];
}

static int
compute_norm(T * q, He * he,
             const real * x, const real * y, const real * z, /**/
             real * normx, real * normy, real * normz)
{
    enum { X, Y, Z };
    int t, nt;
    int i, j, k, nv;
    real a[3], b[3], c[3], u[3], u0[3];
    int *T0, *T1, *T2;
    real theta_a, theta_b, theta_c;

    nt = he_nt(he);
    nv = he_nv(he);
    T0 = q->T0;
    T1 = q->T1;
    T2 = q->T2;

    zero(nv, normx);
    zero(nv, normy);
    zero(nv, normz);

    for (t = 0; t < nt; t++) {
        i = T0[t];
        j = T1[t];
        k = T2[t];
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
        vec_negative(u0, u);    /*This reverses the sign of norm to be inwards */
        vec_set(u, i, /**/ normx, normy, normz);
    }
    return CO_OK;
}

static int
compute_curva_mean(He * he,
                   real * lbx, real * lby, real * lbz,
                   real * normx, real * normy, real * normz,
                   /**/ real * curva_mean)
{
    enum { X, Y, Z };
    int i, nv;
    real u[3], v[3];

    nv = he_nv(he);

    for (i = 0; i < nv; i++) {
        vec_get(i, lbx, lby, lbz, u);
        vec_get(i, normx, normy, normz, v);
        curva_mean[i] = vec_dot(u, v) / 2;
    }

    return CO_OK;

}

real
he_f_gompper_kroll_energy(T * q, He * he,
                          const real * x, const real * y, const real * z)
{
    enum { X, Y, Z };
    int i, j, k, l, v;
    int *T0, *T1, *T2;

    real *len2, *cot;
    real *lbx, *lby, *lbz;
    real *normx, *normy, *normz;
    real *curva_mean;
    real *area;
    real *energy_local;

    real Kb, C0, Kad, DA0D;
    int nv, nt;

    real H0;
    real mH0, mH1, mH2;
    real eng_bend, eng_ad;

    Kb = q->Kb;
    C0 = q->C0;
    Kad = q->Kad;
    DA0D = q->DA0D;

    H0 = C0 / 2.0;

    nv = he_nv(he);
    nt = he_nt(he);

    T0 = q->T0;
    T1 = q->T1;
    T2 = q->T2;
    len2 = q->len2;
    cot = q->cot;
    lbx = q->lbx;
    lby = q->lby;
    lbz = q->lbz;
    normx = q->normx;
    normy = q->normy;
    normz = q->normz;
    curva_mean = q->curva_mean;
    area = q->area;
    energy_local = q->energy_local;


    for (l = 0; l < nt; l++) {
        get_ijk(l, he, /**/ &i, &j, &k);
        T0[l] = i;
        T1[l] = j;
        T2[l] = k;
    }

    if (he_nv(he) != nv)
        ERR(CO_INDEX, "he_nv(he)=%d != nv = %d", he_nv(he), nv);

    compute_len2(he, x, y, z, /**/ len2);
    compute_cot(he, x, y, z, /**/ cot);
    mH0 = compute_area_voronoi(he, len2, cot, /**/ area);
    compute_laplace(he, x, cot, area, /**/ lbx);
    compute_laplace(he, y, cot, area, /**/ lby);
    compute_laplace(he, z, cot, area, /**/ lbz);
    compute_norm(q, he, x, y, z, normx, normy, normz);
    compute_curva_mean(he, lbx, lby, lbz, normx, normy, normz,
                       /**/ curva_mean);

    mH1 = mH2 = 0;
    for (v = 0; v < nv; v++) {
        mH1 += curva_mean[v] * area[v];
        mH2 += curva_mean[v] * curva_mean[v] * area[v];
        energy_local[v] =
            2 * Kb * (curva_mean[v] - H0) * (curva_mean[v] - H0) * area[v];
    }

    eng_bend = 2 * Kb * mH2 - 4 * Kb * H0 * mH1 + 2 * Kb * H0 * H0 * mH0;
    eng_ad =
        2 * pi * Kad * mH1 * mH1 / mH0 - 2 * pi * Kad * DA0D * mH1 / mH0 +
        pi * Kad * DA0D * DA0D / 2 / mH0;

    q->eng_bend = eng_bend;
    q->eng_ad = eng_ad;

    return eng_bend + eng_ad;
}

int
he_f_gompper_kroll_force(T * q, He * he,
                         const real * x, const real * y, const real * z,
                         /**/ real * fx, real * fy, real * fz)
{
    int nv, nt, nh;
    int i, j, k, l;
    int h, n, nn, fnf;
    int *T0, *T1, *T2;

    real Kb, C0, H0;
    real *len2, *cot;
    real *lbx, *lby, *lbz;
    real *normx, *normy, *normz;
    real *curva_mean;
    real *area;
    real cm_integral;

    real cot1, area1, rsq;
    real a[3], b[3], c[3], d[3], r[3];
    real lbi[3], lbisq, lbisq_der[3];
    real coef, coef1, coef2;
    real da1[3], db1[3], dc[3];
    real da2[3], db2[3], dd[3];
    real df[3];

    T0 = q->T0;
    T1 = q->T1;
    T2 = q->T2;
    len2 = q->len2;
    cot = q->cot;
    lbx = q->lbx;
    lby = q->lby;
    lbz = q->lbz;
    normx = q->normx;
    normy = q->normy;
    normz = q->normz;
    curva_mean = q->curva_mean;
    area = q->area;

    nv = q->nv;
    nt = q->nt;
    nh = q->nh;

    Kb = q->Kb;
    C0 = q->C0;
    H0 = C0 / 2.0;

    for (l = 0; l < nt; l++) {
        get_ijk(l, he, /**/ &i, &j, &k);
        T0[l] = i;
        T1[l] = j;
        T2[l] = k;
    }
    compute_len2(he, x, y, z, /**/ len2);
    compute_cot(he, x, y, z, /**/ cot);
    compute_area_voronoi(he, len2, cot, /**/ area);
    compute_laplace(he, x, cot, area, /**/ lbx);
    compute_laplace(he, y, cot, area, /**/ lby);
    compute_laplace(he, z, cot, area, /**/ lbz);
    compute_norm(q, he, x, y, z, normx, normy, normz);
    compute_curva_mean(he, lbx, lby, lbz, normx, normy, normz,
                       /**/ curva_mean);

    cm_integral = 0;
    for (i = 0; i < nv; i++) {
        cm_integral += curva_mean[i] * area[i];
    }

    for (h = 0; h < nh; h++) {

        n = nxt(h);
        nn = nxt(n);
        fnf = flp(nxt(flp(h)));
        i = ver(h);
        j = ver(n);
        k = ver(nn);
        l = ver(fnf);

        cot1 = cot[h];
        area1 = area[i];
        rsq = len2[h];

        get4(x, y, z, i, j, k, l, a, b, c, d);
        get_edg(x, y, z, i, j, r);
        rsq = vec_dot(r, r);

        vec_get(i, lbx, lby, lbz, lbi);
        lbisq = vec_dot(lbi, lbi);

        /*###################################
           ###################################
           force part I
           ###################################
           ################################### */

        /*calculate derivative of Laplace-Beltrami operator: part I */
        /*this is to calculate the derivative of relative position of i, j */

        if (fabs(curva_mean[i]) < epsilon) {
            //printf("__FILE__, __LINE__, curva_mean too small!\n");
            curva_mean[i] = epsilon;
        }
        coef = Kb * (curva_mean[i] - H0) * area[i] / 2.0 / curva_mean[i];

        coef1 = cot1 / area1;
        coef2 = -lbisq * cot1 / area1 / 2.0;

        vec_linear_combination(coef1, lbi, coef2, r, lbisq_der);

        vec_scalar(lbisq_der, coef, df);

        /*accumulate the force on vertices i and j */
        vec_append(df, i, /**/ fx, fy, fz);
        vec_substr(df, j, /**/ fx, fy, fz);

        /*###################################
           ###################################
           force part II
           ###################################
           ################################### */

        /*calculate derivative of Laplace-Beltrami operator: part II */

        /*this is to calculate derivative of cotangent of angle c
           with respect to poisition b, c, a */

        dtri_cot(b, c, a, /**/ db1, dc, da1);

        /*this is calculate derivative of cotangent of angle d
           with respect to poisition a, d, b */

        dtri_cot(a, d, b, /**/ da2, dd, db2);

        coef1 = vec_dot(lbi, r) / area1;
        coef2 = -lbisq * rsq / area1 / 4.0;
        coef *= (coef1 + coef2);

        /*accumulate the force on vertices i, j, k */
        vec_scalar_append(da1, coef, i, /**/ fx, fy, fz);
        vec_scalar_append(db1, coef, j, /**/ fx, fy, fz);
        vec_scalar_append(dc, coef, k, /**/ fx, fy, fz);

        /*accumulate the force on vertices i, j, l */
        vec_scalar_append(da2, coef, i, /**/ fx, fy, fz);
        vec_scalar_append(db2, coef, j, /**/ fx, fy, fz);
        vec_scalar_append(dd, coef, l, /**/ fx, fy, fz);

        /*###################################
           ###################################
           force part III
           ###################################
           ################################### */

        /*calculate derivative of local area with respective to position b, c, a
           This is similiar to the cotangent case above. */
        coef = Kb * 2.0 * (curva_mean[i] - H0) * (curva_mean[i] - H0);

        coef1 = coef * cot1 / 4.0;
        vec_scalar(r, coef1, df);

        /*accumulate the force on vertices i and j */
        vec_append(df, i, /**/ fx, fy, fz);
        vec_substr(df, j, /**/ fx, fy, fz);

        coef2 = coef * rsq / 8.0;

        /*accumulate the force on vertices i, j, k */
        vec_scalar_append(da1, coef2, i, /**/ fx, fy, fz);
        vec_scalar_append(db1, coef2, j, /**/ fx, fy, fz);
        vec_scalar_append(dc, coef2, k, /**/ fx, fy, fz);

        /*accumulate the force on vertices i, j, l */
        vec_scalar_append(da2, coef2, i, /**/ fx, fy, fz);
        vec_scalar_append(db2, coef2, j, /**/ fx, fy, fz);
        vec_scalar_append(dd, coef2, l, /**/ fx, fy, fz);

    }

    return CO_OK;
}

real
he_f_gompper_kroll_energy_ad(T * q)
{
    return q->eng_ad;
}

real
he_f_gompper_kroll_energy_bend(T * q)
{
    return q->eng_bend;
}
