#include <stdio.h>

#include "real.h"
#include "he/memory.h"
#include "he/err.h"
#include "he/he.h"
#include "he/vec.h"
#include "he/tri.h"
#include "he/edg.h"
#include "he/dedg.h"
#include "he/strain.h"
#include "he/f/strain.h"

#define T HeFStrain

static const real EPS = 1e-5;

struct T {
    real *x, *y, *z, *eng;
    Strain *strain;
};

static void zero(int n, real *a) {
    int i;
    for (i = 0; i < n; i++)
        a[i] = 0;
}

static int get_ijk(int t, He *he, /**/ int *pi, int *pj, int *pk) {
    int h, n, nn, i, j, k;
    h = he_hdg_tri(he, t);
    n = he_nxt(he, h);
    nn = he_nxt(he, n);
    i = he_ver(he, h); j = he_ver(he, n); k = he_ver(he, nn);
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

int he_f_strain_ini(const char *name, StrainParam param, const real *x, const real *y, const real *z, He *he, T **pq) {
    T *q;
    int nv, i;

    nv = he_nv(he);
    MALLOC(1, &q);
    MALLOC(nv, &q->x);
    MALLOC(nv, &q->y);
    MALLOC(nv, &q->z);
    MALLOC(nv, &q->eng);
    strain_ini(name, param, &q->strain);

    for (i = 0; i < nv; i++) {
        q->x[i] = x[i];
        q->y[i] = y[i];
        q->z[i] = z[i];
    }

    *pq = q;
    return HE_OK;
}

int he_f_strain_fin(T *q) {
    strain_fin(q->strain);
    FREE(q->x); FREE(q->y); FREE(q->z);
    FREE(q->eng);
    FREE(q);
    return HE_OK;
}

static int small(const real a[3]) { return vec_abs(a) < EPS; }
static int assert_force(const real a[3], const real b[3], const real c[3],
                        const real da[3], const real db[3], const real dc[3]) {
    /* check force and  torque */
    real center[3], m[3], f[3], t[3], ma[3], mb[3], mc[3];
    real ta[3], tb[3], tc[3];
    tri_center(a, b, c, /**/ m);
    vec_minus(a, m, /**/ ma);
    vec_minus(b, m, /**/ mb);
    vec_minus(c, m, /**/ mc);
    vec_mean3(da, db, dc, /**/ f);

    vec_cross(ma, da, /**/ ta);
    vec_cross(mb, db, /**/ tb);
    vec_cross(mc, dc, /**/ tc);
    vec_mean3(ta, tb, tc, /**/ t);

    if (!small(f) || !small(t))  {
        MSG("bad triangle in strain");
        MSG("a, b, c, f, t:");
        vec_fprintf(a, stderr, "%g");
        vec_fprintf(b, stderr, "%g");
        vec_fprintf(c, stderr, "%g");
        vec_fprintf(f, stderr, "%g");
        vec_fprintf(t, stderr, "%g");
        return 0;
    } else
        return 1;
}

int he_f_strain_force(T *q, He *he,
                            const real *x, const real *y, const real *z, /**/
                            real *fx, real *fy, real *fz) {
    real a0[3], b0[3], c0[3];
    real a[3], b[3], c[3], da[3], db[3], dc[3];
    int nt, t;
    int i, j, k;
    nt = he_nt(he);
    for (t = 0; t < nt; t++) {
        get_ijk(t, he, /**/ &i, &j, &k);
        get3(q->x, q->y, q->z, i, j, k, /**/ a0, b0, c0);
        get3(x, y, z, i, j, /**/ k, a, b, c);
        strain_force(q->strain, a0, b0, c0, a, b, c, /**/ da, db, dc);
        if (!assert_force(a, b, c, da, db, dc)) {
            MSG("not force in triangle: %d", t);
            ERR(HE_NUM, "");
        }
        vec_append(da, i, /**/ fx, fy, fz);
        vec_append(db, j, /**/ fx, fy, fz);
        vec_append(dc, k, /**/ fx, fy, fz);
    }
    return HE_OK;
}

real he_f_strain_energy(T *q, He *he,
                        const real *x, const real *y, const real *z) {
    real a0[3], b0[3], c0[3];
    real a[3], b[3], c[3];
    int nt, nv, t;
    int i, j, k;
    real e0, e, *eng;
    nt = he_nt(he);
    nv = he_nv(he);

    e = 0;
    eng = q->eng;
    zero(nv, eng);
    for (t = 0; t < nt; t++) {
        get_ijk(t, he, /**/ &i, &j, &k);
        get3(q->x, q->y, q->z, i, j, k, /**/ a0, b0, c0);
        get3(x, y, z, i, j, /**/ k, a, b, c);
        e0 = strain_energy(q->strain, a0, b0, c0, a, b, c);
        eng[i] += e0/3;
        eng[j] += e0/3;
        eng[k] += e0/3;
        e += e0;
    }
    return e;
}


int he_f_strain_energy_ver(T *q, /**/ real **pa) {
    *pa = q->eng;
    return HE_OK;
}
