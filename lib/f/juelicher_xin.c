#include <stdio.h>
#include <math.h>

#include "real.h"
#include "co/argv.h"
#include "co/memory.h"
#include "co/err.h"
#include "co/he.h"
#include "co/vec.h"
#include "co/i/vec.h"
#include "co/dih.h"
#include "co/ddih.h"
#include "co/dedg.h"
#include "co/tri.h"
#include "co/dtri.h"
#include "co/sum.h"

#include "co/f/juelicher_xin.h"

#define T HeFJuelicherXin

#define  nxt(h)     he_nxt(he, h)
#define  flp(h)     he_flp(he, h)
#define  ver(h)     he_ver(he, h)
#define  hdg_ver(v) he_hdg_ver(he, v)
#define  hdg_edg(e) he_hdg_edg(he, e)
#define  hdg_tri(t) he_hdg_tri(he, t)
#define  bnd(h)     he_bnd(he, h)

static const real pi = 3.141592653589793115997964;

typedef struct Size Size;
struct Size {
    int nv, ne, nt;
};

typedef struct Param Param;
struct Param {
    real K, H0, Kad, DA0D;
};

struct T {
    Size size;
    Param param;
    real *theta, *len, *len_theta, *area, *H;
    real *energy, *fx, *fy, *fz, *fxad, *fyad, *fzad;

    real eng_bend, eng_ad;
};

static void
zero(int n, real * a)
{
    int i;

    for (i = 0; i < n; i++)
        a[i] = 0;
}

static real
sum(int n, const real * a)
{
    int i;
    real s;
    HeSum *sum;

    he_sum_ini(&sum);
    s = 0;
    for (i = 0; i < n; i++)
        he_sum_add(sum, a[i]);
    s = he_sum_get(sum);
    he_sum_fin(sum);
    return s;
}

static int
divide(int n, const real * a, const real * b, /**/ real * c)
{
    int i;

    for (i = 0; i < n; i++)
        c[i] = a[i] / b[i];
    return CO_OK;
}

static int
plus(int n, const real * a, /*io */ real * b)
{
    int i;

    for (i = 0; i < n; i++)
        b[i] += a[i];
    return CO_OK;
}

static int
scale(int n, real sc, /*io */ real * a)
{
    int i;

    for (i = 0; i < n; i++)
        a[i] *= sc;
    return CO_OK;
}

int
he_f_juelicher_xin_ini(real K, real C0, real Kad, real DA0D,
                       He * he, T ** pq)
{
    T *q;
    int nv, ne, nt;
    Size size;
    Param param;

    MALLOC(1, &q);

    nv = he_nv(he);
    ne = he_ne(he);
    nt = he_nt(he);

    param.K = K;
    param.H0 = C0 / 2;
    param.Kad = Kad;
    param.DA0D = DA0D;

    q->param = param;

    size.nv = nv;
    size.nt = nt;
    size.ne = ne;
    q->size = size;

    MALLOC(nv, &q->len_theta);
    MALLOC(nv, &q->H);

    MALLOC(nv, &q->area);
    MALLOC(nv, &q->energy);
    MALLOC(ne, &q->theta);
    MALLOC(ne, &q->len);

    MALLOC(nv, &q->fx);
    MALLOC(nv, &q->fy);
    MALLOC(nv, &q->fz);
    MALLOC(nv, &q->fxad);
    MALLOC(nv, &q->fyad);
    MALLOC(nv, &q->fzad);

    *pq = q;

    return CO_OK;
}

int
he_f_juelicher_xin_argv(char ***p, He * he, T ** pq)
{
    int status;
    real Kb, C0, Kad, DA0D;

    if ((status = argv_real(p, &Kb)) != CO_OK)
        return status;
    if ((status = argv_real(p, &C0)) != CO_OK)
        return status;
    if ((status = argv_real(p, &Kad)) != CO_OK)
        return status;
    if ((status = argv_real(p, &DA0D)) != CO_OK)
        return status;
    return he_f_juelicher_xin_ini(Kb, C0, Kad, DA0D, he, pq);
}

int
he_f_juelicher_xin_fin(T * q)
{
    FREE(q->len_theta);
    FREE(q->H);
    FREE(q->area);
    FREE(q->energy);
    FREE(q->theta);
    FREE(q->len);

    FREE(q->fx);
    FREE(q->fy);
    FREE(q->fz);
    FREE(q->fxad);
    FREE(q->fyad);
    FREE(q->fzad);
    FREE(q);
    return CO_OK;
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
    i_vec_get(i, x, y, z, /**/ a);
    i_vec_get(j, x, y, z, /**/ b);
    i_vec_get(k, x, y, z, /**/ c);
    return CO_OK;
}

static int
get2(int h, He * he, const real * x, const real * y, const real * z,
     /**/ real a[3], real b[3])
{
    int f, i, j;

    f = flp(h);
    i = ver(h);
    j = ver(f);
    vec_get(i, x, y, z, /**/ a);
    vec_get(j, x, y, z, /**/ b);
    return CO_OK;
}

enum { BULK, BND };
static int
get_ijkl(int h, He * he, /**/ int *pi, int *pj, int *pk, int *pl)
{
    int n, nn, nnf, i, j, k, l;

    n = nxt(h);
    nn = nxt(nxt(h));
    nnf = nxt(nxt(flp(h)));
    j = ver(h);
    k = ver(n);
    i = ver(nn);
    l = ver(nnf);

    *pi = i;
    *pj = j;
    *pk = k;
    *pl = l;
    return BULK;
}

static int
get_ij(int h, He * he, /**/ int *pi, int *pj)
{
    int f, i, j;

    f = flp(h);
    i = ver(h);
    j = ver(f);
    *pi = i;
    *pj = j;
    return CO_OK;
}

static int
get4(const real * x, const real * y, const real * z,
     int i, int j, int k, int l,
     /**/ real a[3], real b[3], real c[3], real d[3])
{
    vec_get(i, x, y, z, /**/ a);
    vec_get(j, x, y, z, /**/ b);
    vec_get(k, x, y, z, /**/ c);
    vec_get(l, x, y, z, /**/ d);
    return CO_OK;
}

static int
compute_area(He * he, Size size, const real * xx, const real * yy,
             const real * zz, /**/ real * area)
{
    int t, i, j, k;
    int nv, nt;
    real a[3], b[3], c[3];
    real area0;

    nt = size.nt;
    nv = size.nv;

    zero(nv, area);
    for (t = 0; t < nt; t++) {
        get_ijk(t, he, &i, &j, &k);
        get3(xx, yy, zz, i, j, k, a, b, c);
        area0 = tri_area(a, b, c) / 3;
        area[i] += area0;
        area[j] += area0;
        area[k] += area0;
    }
    return CO_OK;
}

static int
compute_len(He * he, Size size, const real * xx, const real * yy,
            const real * zz, /**/ real * len)
{
    int ne, e, h;
    real b[3], c[3], u[3];

    ne = size.ne;

    for (e = 0; e < ne; e++) {
        h = hdg_edg(e);
        if (bnd(h))
            continue;
        get2(h, he, xx, yy, zz, /**/ c, b);
        vec_minus(c, b, /**/ u);
        len[e] = vec_abs(u);
    }
    return CO_OK;
}

static int
compute_theta(He * he, Size size, const real * xx, const real * yy,
              const real * zz, /**/ real * theta)
{
    int ne, e, h;
    int i, j, k, l;
    real a[3], b[3], c[3], d[3];

    ne = size.ne;
    for (e = 0; e < ne; e++) {
        h = hdg_edg(e);
        if (bnd(h))
            continue;
        get_ijkl(h, he, /**/ &i, &j, &k, &l);
        get4(xx, yy, zz, i, j, k, l, /**/ a, b, c, d);
        theta[e] = dih_angle_sup(a, b, c, d);
    }
    return CO_OK;
}

static int
compute_len_theta(He * he, Size size, real * len, real * theta,
                  /**/ real * len_theta)
{
    int nv, ne, e, h;
    int i, j;
    real cur;

    nv = size.nv;
    ne = size.ne;

    zero(nv, len_theta);
    for (e = 0; e < ne; e++) {
        h = hdg_edg(e);
        get_ij(h, he, /**/ &i, &j);
        cur = len[e] * theta[e];
        len_theta[i] += cur;
        len_theta[j] += cur;
    }
    return CO_OK;
}

static int
compute_energy_local(Param param, int nv, real * area, real * H,
                     /**/ real * energy)
{
    real H0;
    int v;

    H0 = param.H0;
    for (v = 0; v < nv; v++)
        energy[v] = (H[v] - H0) * (H[v] - H0) * area[v];
    return CO_OK;
}

real
he_f_juelicher_xin_energy(T * q, He * he,
                          const real * x, const real * y, const real * z)
{
    Size size;
    Param param;
    real K, Kad, DA0D;
    real *area, *H, *energy, *theta, *len, *len_theta;
    real area_tot, len_theta_tot, scurv;
    real eng_bend, eng_ad;
    int nv;

    size = q->size;

    param = q->param;
    K = param.K;
    Kad = param.Kad;
    DA0D = param.DA0D;

    area = q->area;
    H = q->H;
    energy = q->energy;
    len = q->len;
    theta = q->theta;
    len_theta = q->len_theta;

    nv = size.nv;

    compute_len(he, size, x, y, z, /**/ len);
    compute_theta(he, size, x, y, z, /**/ theta);
    compute_len_theta(he, size, len, theta, /**/ len_theta);
    compute_area(he, size, x, y, z, /**/ area);

    divide(nv, len_theta, area, /**/ H);
    scale(nv, 0.25, /**/ H);

    compute_energy_local(param, nv, area, H, /**/ energy);
    scale(nv, 2 * K, energy);
    eng_bend = sum(nv, energy);

    /* Ead */
    area_tot = sum(nv, area);
    len_theta_tot = sum(nv, len_theta);
    scurv = (len_theta_tot / 2 - DA0D) / area_tot;

    eng_ad = pi * Kad * area_tot * scurv * scurv / 2;

    q->eng_bend = eng_bend;
    q->eng_ad = eng_ad;

    return eng_bend + eng_ad;
}

static int
f_len(Param param, He * he, Size size,
      const real * theta, const real * H,
      const real * xx, const real * yy, const real * zz, /**/
      real * fx, real * fy, real * fz)
{
    int h, e, i, j;
    int ne;
    real coef, H0;
    real b[3], c[3], db[3], dc[3];

    ne = size.ne;
    H0 = param.H0;

    for (e = 0; e < ne; e++) {
        h = hdg_edg(e);
        get_ij(h, he, /**/ &i, &j);
        vec_get(i, xx, yy, zz, /**/ b);
        vec_get(j, xx, yy, zz, /**/ c);
        dedg_abs(b, c, db, dc);
        coef = (H[i] + H[j] - 2 * H0) * theta[e];
        vec_scalar_append(db, coef, i, fx, fy, fz);
        vec_scalar_append(dc, coef, j, fx, fy, fz);
    }
    return CO_OK;
}

static int
f_theta(Param param, He * he, Size size,
        const real * len, const real * H,
        const real * xx, const real * yy, const real * zz,
        /**/ real * fx, real * fy, real * fz)
{
    int h, e, ne;
    int i, j, k, l;
    real coef, H0;
    real a[3], b[3], c[3], d[3];
    real da[3], db[3], dc[3], dd[3];

    ne = size.ne;
    H0 = param.H0;

    for (e = 0; e < ne; e++) {
        h = hdg_edg(e);
        if (bnd(h))
            continue;
        get_ijkl(h, he, /**/ &i, &j, &k, &l);
        get4(xx, yy, zz, i, j, k, l, /**/ a, b, c, d);
        ddih_angle_sup(a, b, c, d, da, db, dc, dd);
        coef = (H[j] + H[k] - 2 * H0) * len[e];
        vec_scalar_append(da, coef, i, fx, fy, fz);
        vec_scalar_append(db, coef, j, fx, fy, fz);
        vec_scalar_append(dc, coef, k, fx, fy, fz);
        vec_scalar_append(dd, coef, l, fx, fy, fz);
    }
    return CO_OK;
}

static int
f_area(Param param, He * he, Size size, const real * H,
       const real * xx, const real * yy, const real * zz,
       /**/ real * fx, real * fy, real * fz)
{
    int nt, t, i, j, k;
    real a[3], b[3], c[3];
    real da[3], db[3], dc[3];
    real coef, H0;

    nt = size.nt;
    H0 = param.H0;
    for (t = 0; t < nt; t++) {
        get_ijk(t, he, &i, &j, &k);
        get3(xx, yy, zz, i, j, k, a, b, c);
        dtri_area(a, b, c, da, db, dc);
        coef =
            -2 * (H[i] * H[i] + H[j] * H[j] + H[k] * H[k] -
                  3 * H0 * H0) / 3;
        vec_scalar_append(da, coef, i, fx, fy, fz);
        vec_scalar_append(db, coef, j, fx, fy, fz);
        vec_scalar_append(dc, coef, k, fx, fy, fz);
    }
    return CO_OK;
}

static int
fad_len(He * he, Size size, real coef, const real * theta,
        const real * xx, const real * yy, const real * zz, /**/
        real * fx, real * fy, real * fz)
{
    int h, e, i, j;
    int ne;
    real b[3], c[3], db[3], dc[3];

    ne = size.ne;
    for (e = 0; e < ne; e++) {
        h = hdg_edg(e);
        get_ij(h, he, /**/ &i, &j);
        vec_get(i, xx, yy, zz, /**/ b);
        vec_get(j, xx, yy, zz, /**/ c);
        dedg_abs(b, c, db, dc);
        vec_scalar_append(db, coef * theta[e], i, fx, fy, fz);
        vec_scalar_append(dc, coef * theta[e], j, fx, fy, fz);
    }
    return CO_OK;
}

static int
fad_theta(He * he, Size size, real coef,
          const real * len,
          const real * xx, const real * yy, const real * zz,
          /**/ real * fx, real * fy, real * fz)
{
    int h, e, ne;
    int i, j, k, l;
    real a[3], b[3], c[3], d[3];
    real da[3], db[3], dc[3], dd[3];

    ne = size.ne;
    for (e = 0; e < ne; e++) {
        h = hdg_edg(e);
        if (bnd(h))
            continue;
        get_ijkl(h, he, /**/ &i, &j, &k, &l);
        get4(xx, yy, zz, i, j, k, l, /**/ a, b, c, d);
        ddih_angle_sup(a, b, c, d, da, db, dc, dd);
        vec_scalar_append(da, coef * len[e], i, fx, fy, fz);
        vec_scalar_append(db, coef * len[e], j, fx, fy, fz);
        vec_scalar_append(dc, coef * len[e], k, fx, fy, fz);
        vec_scalar_append(dd, coef * len[e], l, fx, fy, fz);
    }
    return CO_OK;
}

static int
fad_area(He * he, Size size, real coef,
         const real * xx, const real * yy, const real * zz,
         /**/ real * fx, real * fy, real * fz)
{
    int nt, t, i, j, k;
    real a[3], b[3], c[3];
    real da[3], db[3], dc[3];

    nt = size.nt;
    for (t = 0; t < nt; t++) {
        get_ijk(t, he, &i, &j, &k);
        get3(xx, yy, zz, i, j, k, a, b, c);
        dtri_area(a, b, c, da, db, dc);
        vec_scalar_append(da, coef, i, fx, fy, fz);
        vec_scalar_append(db, coef, j, fx, fy, fz);
        vec_scalar_append(dc, coef, k, fx, fy, fz);
    }
    return CO_OK;
}


int
he_f_juelicher_xin_force(T * q, He * he,
                         const real * x, const real * y, const real * z,
                         /**/ real * fx_tot, real * fy_tot, real * fz_tot)
{
    Size size;
    Param param;
    int nv;
    real K, Kad, DA0D;
    real *theta, *len, *area, *len_theta, *H;
    real area_tot, len_theta_tot, scurv;
    real *fx, *fy, *fz, *fxad, *fyad, *fzad;

    param = q->param;
    size = q->size;

    K = param.K;
    DA0D = param.DA0D;
    Kad = param.Kad;

    area = q->area;
    theta = q->theta;
    len = q->len;
    len_theta = q->len_theta;
    H = q->H;
    fx = q->fx;
    fy = q->fy;
    fz = q->fz;
    fxad = q->fxad;
    fyad = q->fyad;
    fzad = q->fzad;

    nv = size.nv;

    zero(nv, fx);
    zero(nv, fy);
    zero(nv, fz);
    zero(nv, fxad);
    zero(nv, fyad);
    zero(nv, fzad);

    compute_len(he, size, x, y, z, /**/ len);
    compute_theta(he, size, x, y, z, /**/ theta);
    compute_len_theta(he, size, len, theta, /**/ len_theta);
    compute_area(he, size, x, y, z, /**/ area);
    divide(nv, len_theta, area, /**/ H);
    scale(nv, 0.25, /**/ H);

    f_len(param, he, size, theta, H, x, y, z, /**/ fx, fy, fz);
    f_theta(param, he, size, len, H, x, y, z, /**/ fx, fy, fz);
    f_area(param, he, size, H, x, y, z, /**/ fx, fy, fz);
    scale(nv, K, fx);
    scale(nv, K, fy);
    scale(nv, K, fz);
    plus(nv, fx, /*io */ fx_tot);
    plus(nv, fy, /*io */ fy_tot);
    plus(nv, fz, /*io */ fz_tot);

    area_tot = sum(nv, area);
    len_theta_tot = sum(nv, len_theta);
    scurv = (len_theta_tot / 2 - DA0D) / area_tot;

    fad_len(he, size, scurv, theta, x, y, z, /**/ fxad, fyad, fzad);
    fad_theta(he, size, scurv, len, x, y, z, /**/ fxad, fyad, fzad);
    fad_area(he, size, -scurv * scurv / 2, x, y, z, /**/ fxad, fyad, fzad);
    scale(nv, pi * Kad, fxad);
    scale(nv, pi * Kad, fyad);
    scale(nv, pi * Kad, fzad);

    plus(nv, fxad, /*io */ fx_tot);
    plus(nv, fyad, /*io */ fy_tot);
    plus(nv, fzad, /*io */ fz_tot);
    return CO_OK;
}

int
he_f_juelicher_xin_curva_mean_ver(T * q, /**/ real ** pa)
{
    *pa = q->H;
    return CO_OK;
}

int
he_f_juelicher_xin_energy_ver(T * q, /**/ real ** pa)
{
    *pa = q->energy;
    return CO_OK;
}

int
he_f_juelicher_xin_area_ver(T * q, /**/ real ** pa)
{
    *pa = q->area;
    return CO_OK;
}

real
he_f_juelicher_xin_energy_ad(T * q)
{
    return q->eng_ad;
}

real
he_f_juelicher_xin_energy_bend(T * q)
{
    return q->eng_bend;
}
