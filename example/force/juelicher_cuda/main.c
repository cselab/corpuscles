#include <stdio.h>
#include <math.h>

#include <real.h>
#include <he/err.h>
#include <he/x.h>
#include <he/vec.h>
#include <he/tri.h>
#include <he/memory.h>
#include <he/punto.h>
#include <he/ddih.h>
#include <he/dedg.h>
#include <he/dtri.h>

void get3(int i, int j, int k, /**/ real a[3], real b[3], real c[3]) {
    vec_get(i, XX, YY, ZZ, a);
    vec_get(j, XX, YY, ZZ, b);
    vec_get(k, XX, YY, ZZ, c);
}

void get2(int i, int j, /**/ real a[3], real b[3]) {
    vec_get(i, XX, YY, ZZ, a);
    vec_get(j, XX, YY, ZZ, b);
}

void get4(int i, int j, int k, int l, /**/
          real a[3], real b[3], real c[3], real d[3]) {
    vec_get(i, XX, YY, ZZ, a);
    vec_get(j, XX, YY, ZZ, b);
    vec_get(k, XX, YY, ZZ, c);
    vec_get(l, XX, YY, ZZ, d);
}

static void write(real *fx, real *fy, real *fz,
                  real *fxad, real *fyad, real *fzad,
                  real *area) {
    real *queue[] = {XX, YY, ZZ, fx, fy, fz, fxad, fyad, fzad, area, NULL};
    punto_fwrite(NV, queue, stdout);
}

static void compute_theta_len(/**/ real *theta, real *lentheta, real *plentheta_tot) {
    int e, he, t;
    int i, j, k, l;
    real a[3], b[3], c[3], d[3], u[3];
    real len0, theta0, lentheta0, area0;
    real lentheta_tot;

    lentheta_tot = 0;
    for (e = 0; e < NE; e++) {
        he = hdg_edg(e);
        if ( bnd(he) ) continue;
        i = D0[e]; j = D1[e]; k = D2[e]; l = D3[e];
        get4(i, j, k, l, /**/ a, b, c, d);
        theta[e] = theta0 = tri_dih(a, b, c, d);
        vec_minus(c, b, u);
        len0 = vec_abs(u);
        lentheta0    = len0*theta0;
        lentheta[j] += lentheta0;
        lentheta[k] += lentheta0;
        lentheta_tot += lentheta0;
    }
    *plentheta_tot = lentheta_tot/2;
}

static void compute_area(/**/ real *area, real *parea_tot) {
    int t, i, j, k;
    real area0;
    real a[3], b[3], c[3], d[3];
    real area_tot;

    area_tot = 0;
    for (t = 0; t < NT; t++) {
        i = T0[t]; j = T1[t]; k = T2[t];
        get3(i, j, k, a, b, c);
        area0 = tri_area(a, b, c);
        area[i] += area0/3;
        area[j] += area0/3;
        area[k] += area0/3;
        area_tot += area0;
    }

    *parea_tot = area_tot;
}

static void compute_mean_curv(real H0, real kb, real lentheta, real area, /**/ real *pmean) {
    real mean;
    real kad;

    kad = 2.0 * kb / pi;
    mean = lentheta - H0 *area;
    mean = mean * (4 * kad * pi / area);

    *pmean = mean;
}

static void force_edg(real H0, real curva_mean_area_tot, const real *theta,  const real *lentheta, const real *area,  /**/
                      real *fx, real *fy, real *fz,
                      real *fxad, real *fyad, real *fzad) {
    int e, he, j, k;
    real theta0, coef;
    real b[3], c[3], db[3], dc[3];

    for (e = 0; e < NE; e++) {
        he = hdg_edg(e);
        j = D1[e]; k = D2[e];
        get2(j, k, /**/ b, c);
        dedg_abs(b, c, db, dc);

        theta0 = theta[e];
        coef = - ( (lentheta[j]/area[j]/4 - H0) + (lentheta[k]/area[k]/4 - H0) ) * theta0;
        vec_scalar_append(db, coef, j, fx, fy, fz);
        vec_scalar_append(dc, coef, k, fx, fy, fz);
        coef = -curva_mean_area_tot/4 * theta0;
        vec_scalar_append(db, coef, j, fxad, fyad, fzad);
        vec_scalar_append(dc, coef, k, fxad, fyad, fzad);
    }
}

static void force_lentheta(real H0, real curva_mean_area_tot, const real *lentheta, const real *area,
                           real *fx, real *fy, real *fz,
                           real *fxad, real *fyad, real *fzad) {
    int e, he;
    int i, j, k, l;
    real len0, coef;
    real a[3], b[3], c[3], d[3];
    real da[3], db[3], dc[3], dd[3], u[3];

    for (e = 0; e < NE; e++) {
        he = hdg_edg(e);
        i = D0[e]; j = D1[e]; k = D2[e]; l = D3[e];
        get4(i, j, k, l, /**/ a, b, c, d);
        ddih_angle(a, b, c, d, da, db, dc, dd);
        vec_minus(c, b, u);
        len0 = vec_abs(u);

        coef =  -(  (lentheta[j]/area[j]/4 - H0) + (lentheta[k]/area[k]/4 - H0) ) * len0 ;

        vec_scalar_append(da, coef, i, fx, fy, fz);
        vec_scalar_append(db, coef, j, fx, fy, fz);
        vec_scalar_append(dc, coef, k, fx, fy, fz);
        vec_scalar_append(dd, coef, l, fx, fy, fz);

        coef = -curva_mean_area_tot/4.0 *len0;
        vec_scalar_append(da, coef, i, fxad, fyad, fzad);
        vec_scalar_append(db, coef, j, fxad, fyad, fzad);
        vec_scalar_append(dc, coef, k, fxad, fyad, fzad);
        vec_scalar_append(dd, coef, l, fxad, fyad, fzad);

    }
}

static void force_area(real H0, const real *lentheta, const real *area,
                       real *fx, real *fy, real *fz,
                       real *fxad, real *fyad, real *fzad) {
    int t, i, j, k;
    real a[3], b[3], c[3];
    real da[3], db[3], dc[3];
    real coef1, coef2, coef;

    for (t = 0; t < NT; t++) {
        i = T0[t]; j = T1[t]; k = T2[t];

        get3(i, j, k, a, b, c);
        dtri_area(a, b, c, da, db, dc);

        coef1 = 1.0/3.0;

        coef2 = lentheta[i]*lentheta[i]/8.0/area[i]/area[i] - 2.0*H0*H0;
        coef = coef1 * coef2;
        vec_scalar_append(da, coef, i, fx, fy, fz);

        coef2 = lentheta[j]*lentheta[j]/8.0/area[j]/area[j] - 2.0*H0*H0;
        coef = coef1 * coef2;
        vec_scalar_append(db, coef, j, fx, fy, fz);

        coef2 = lentheta[k]*lentheta[k]/8.0/area[k]/area[k] - 2.0*H0*H0;
        coef = coef1 * coef2;
        vec_scalar_append(dc, coef, k, fx, fy, fz);
    }
}

void force_juelicher() {
    real kb, C0, H0, kad;
    real area_tot, lentheta_tot, curva_mean_area_tot;
    real *lentheta, *area;
    real *theta;
    real *fx, *fy, *fz;
    real *fxad, *fyad, *fzad;

    RZERO(NV, &lentheta);
    RZERO(NV, &area);

    RZERO(NV, &fx);
    RZERO(NV, &fy);
    RZERO(NV, &fz);

    RZERO(NV, &fxad);
    RZERO(NV, &fyad);
    RZERO(NV, &fzad);

    RZERO(NE, &theta);

    kb  = 1.0;
    C0  = -1.0;
    H0  = C0/2.0;
    kad = 2.0 * kb / pi;

    compute_theta_len(/**/ theta, lentheta, &lentheta_tot);
    compute_area(/**/ area, &area_tot);

    compute_mean_curv(H0, kb, lentheta_tot, area_tot, /**/ &curva_mean_area_tot);

    force_edg(H0, curva_mean_area_tot,   theta,  lentheta, area,  /*io*/ fx, fy, fz, fxad, fyad, fzad);
    force_lentheta(H0, curva_mean_area_tot, lentheta, area, /*io*/ fx, fy, fz, fxad, fyad, fzad);
    force_area(H0, lentheta, area, /*io*/ fx, fy, fz, fxad, fyad, fzad);
    write(fx, fy, fz, fxad, fyad, fzad, area);

    FREE(lentheta);
    FREE(area);
    FREE(fx);
    FREE(fy);
    FREE(fz);
    FREE(fxad);
    FREE(fyad);
    FREE(fzad);
    FREE(theta);
}

int main() {
    ini("/dev/stdin");
    force_juelicher();
    fin();
    return HE_OK;
}
