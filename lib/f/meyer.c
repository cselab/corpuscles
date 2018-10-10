#include <stdio.h>
#include <math.h>

#include "real.h"
#include "he/memory.h"
#include "he/err.h"
#include "he/he.h"
#include "he/vec.h"
#include "he/dih.h"
#include "he/ddih.h"
#include "he/tri.h"

#include "he/f/meyer.h"

#define T HeFMeyer

#    define  nxt(h)     he_nxt(he, h)
#    define  flp(h)     he_flp(he, h)
#    define  ver(h)     he_ver(he, h)
#    define  hdg_ver(v) he_hdg_ver(he, v)
#    define  hdg_edg(e) he_hdg_edg(he, e)
#    define  hdg_tri(t) he_hdg_tri(he, t)
#    define  bnd(h)     he_bnd(he, h)

static const real pi = 3.141592653589793115997964;

struct T {
    real K, C0, Kad;
    real *lbx, *lby, *lbz, *curva_mean, *energy, *area;
    int *T0, *T1, *T2;
};

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

int he_f_meyer_ini(real K, real C0, real Kad, He *he, T **pq) {
    T *q;
    int nv, nt;
    MALLOC(1, &q);
    nv = he_nv(he);
    nt = he_nt(he);

    q->K = K;
    q->C0 = C0;
    q->Kad = Kad;

    MALLOC(nt, &q->T0); MALLOC(nt, &q->T1); MALLOC(nt, &q->T2);
    MALLOC(nv, &q->lbx); MALLOC(nv, &q->lby); MALLOC(nv, &q->lbz);
    MALLOC(nv, &q->curva_mean); MALLOC(nv, &q->energy); MALLOC(nv, &q->area);

    *pq = q;
    return HE_OK;
}

int he_f_meyer_fin(T *q) {
    FREE(q->lbx); FREE(q->lby); FREE(q->lbz);
    FREE(q->curva_mean); FREE(q->energy); FREE(q->area);
    return HE_OK;
}

enum {BULK, BND};
static int get_ijkl(int e, He *he, /**/ int *pi, int *pj, int *pk, int *pl) {
#    define  nxt(h)     he_nxt(he, h)
#    define  flp(h)     he_flp(he, h)
#    define  ver(h)     he_ver(he, h)
#    define  hdg_ver(v) he_hdg_ver(he, v)
#    define  hdg_edg(e) he_hdg_edg(he, e)
#    define  bnd(h)     he_bnd(he, h)
    int h, n, nn, nnf, i, j, k, l;
    h = he_hdg_edg(he, e);
    if (bnd(h)) return BND;

    h = hdg_edg(e); n = nxt(h); nn = nxt(nxt(h));
    nnf = nxt(nxt(flp(h)));
    j = ver(h); k = ver(n); i = ver(nn); l = ver(nnf);

    *pi = i; *pj = j; *pk = k; *pl = l;
    return BULK;
}

static int get(int e, He *he, const real *x, const real *y, const real *z,
               /**/ real a[3], real b[3], real c[3], real d[3]) {
    int status, i, j, k, l;
    status = get_ijkl(e, he, /**/ &i, &j, &k, &l);
    if (status == BND) return BND;
    vec_get(i, x, y, z, /**/ a);
    vec_get(j, x, y, z, /**/ b);
    vec_get(k, x, y, z, /**/ c);
    vec_get(l, x, y, z, /**/ d);
    return BULK;
}

static void compute_force(real K,
                          He *he, const real *x, const real *y, const real *z, /**/
                          real *fx, real *fy, real *fz) {
    int status, n, t, i, j, k, l;
    real a[3], b[3], c[3], d[3], da[3], db[3], dc[3], dd[3];
    n = he_ne(he);
    for (t = 0; t < n; t++) {
        status = get_ijkl(t, he, /**/ &i, &j, &k, &l);
        if (status == BND) continue;
        vec_get(i, x, y, z, /**/ a);
        vec_get(j, x, y, z, /**/ b);
        vec_get(k, x, y, z, /**/ c);
        vec_get(l, x, y, z, /**/ d);
        ddih_cos(a, b, c, d, /**/ da, db, dc, dd);
        vec_scalar_append(da, 2*K, i, /**/ fx, fy, fz);
        vec_scalar_append(db, 2*K, j, /**/ fx, fy, fz);
        vec_scalar_append(dc, 2*K, k, /**/ fx, fy, fz);
        vec_scalar_append(dd, 2*K, l, /**/ fx, fy, fz);
    }
}

 int he_f_meyer_force(T *q, He *he,
                     const real *x, const real *y, const real *z, /**/
                     real *fx, real *fy, real *fz) {
    return HE_OK;
}

real he_f_meyer_energy(T *q, He *he,
                       const real *XX, const real *YY, const real *ZZ) {
    int n;
    real *acos, K;
    int v, t;
    int i, j, k;
    real a[3], b[3], c[3], u[3], coord[3];
    real *lbx, *lby, *lbz, *curva_mean, *energy, *area;
    real len, area0;
    real rxy, phi;
    int  obtuse;
    real theta0;
    real cota,cotb,cotc;
    real ab2, bc2, ca2;
    real area_tot_tri, area_tot_mix;
    real energy_tot;
    int NV, NT;
    int *T0, *T1, *T2;
    
    K  = q->K;
    NV = he_nv(he);
    NT = he_nt(he);

    lbx = q->lbx; lby = q->lby; lbz = q->lbz;
    curva_mean = q->curva_mean;
    energy = q->energy;
    area = q->area;
    T0 = q->T0; T1 = q->T1; T2 = q->T2;

    for (t = 0; t < NT; t++) {
        get_ijk(t, he, /**/ &i, &j, &k);
        T0[t] = i; T1[t] = j; T2[t] = k;
    }

    for (v = 0; v < NV; v++) {
        lbx[v] = 0; lby[v] = 0; lbz[v] = 0;
        curva_mean[v] = 0; energy[v] = 0; area[v] = 0;
    }

    area_tot_tri = 0;
    for ( t = 0; t < NT; t++ ) {
        /*At first, assume it is not an obtuse triangle*/
        obtuse = 0;

        i = T0[t]; j = T1[t]; k = T2[t];

        get3(XX, YY, ZZ, i, j, k, a, b, c);
        area0 = tri_area(a, b, c);

        area_tot_tri += area0;

        cota = tri_cot(c, a, b);
        cotb = tri_cot(a, b, c);
        cotc = tri_cot(b, c, a);

        vec_minus(a, b,  u);

        lbx[i] += cotc*u[0]/2;
        lby[i] += cotc*u[1]/2;
        lbz[i] += cotc*u[2]/2;

        lbx[j] -= cotc*u[0]/2;
        lby[j] -= cotc*u[1]/2;
        lbz[j] -= cotc*u[2]/2;

        ab2 = vec_dot(u, u);

        vec_minus(b, c, u);

        lbx[j] += cota*u[0]/2;
        lby[j] += cota*u[1]/2;
        lbz[j] += cota*u[2]/2;

        lbx[k] -= cota*u[0]/2;
        lby[k] -= cota*u[1]/2;
        lbz[k] -= cota*u[2]/2;

        bc2 = vec_dot(u, u);

        vec_minus(c, a,  u);

        lbx[k] += cotb*u[0]/2;
        lby[k] += cotb*u[1]/2;
        lbz[k] += cotb*u[2]/2;

        lbx[i] -= cotb*u[0]/2;
        lby[i] -= cotb*u[1]/2;
        lbz[i] -= cotb*u[2]/2;

        ca2 = vec_dot(u, u);

        /*check if angle a is obtuse*/
        theta0 = tri_angle(c, a, b);

        if ( theta0 > pi/2.0 ) {

            area[i] += area0/2;
            area[j] += area0/4;
            area[k] += area0/4;

            obtuse = 1;
        }
        else {
            /*check if angle b is obtuse*/
            theta0=tri_angle(a, b, c);

            if ( theta0 > pi/2.0 ) {

                area[j] += area0/2;
                area[i] += area0/4;
                area[k] += area0/4;

                obtuse = 1;
            }
            else {
                /*check if angle c is obtuse*/
                theta0=tri_angle(b, c, a);

                if ( theta0 > pi/2.0 ) {

                    area[k] += area0/2;
                    area[i] += area0/4;
                    area[j] += area0/4;

                    obtuse = 1;
                }

            }

        }

        if ( obtuse == 0 ) {

            area[i] += ( ab2*cotc + ca2*cotb ) / 8;
            area[j] += ( bc2*cota + ab2*cotc ) / 8;
            area[k] += ( ca2*cotb + bc2*cota ) / 8;

        }

    }

    area_tot_mix = 0;
    energy_tot   = 0;

    for ( v = 0; v < NV; v++ ) {
        lbx[v] /=area[v];
        lby[v] /=area[v];
        lbz[v] /=area[v];

        u[0] = lbx[v];
        u[1] = lby[v];
        u[2] = lbz[v];
        len = vec_dot(u, u);
        curva_mean[v] = len;
        len = sqrt(len);
        curva_mean[v] /= len;
        curva_mean[v] /= 2;

        energy[v] = 2 * curva_mean[v]* curva_mean[v] * area[v];

        /*for verification*/
        area_tot_mix += area[v];
        energy_tot   += energy[v];

        vec_get(v, XX, YY, ZZ, coord);
        rxy = vec_cylindrical_r(coord);
    }
    return energy_tot;
}
