#include <stdio.h>

#include "real.h"

#include "he/argv.h"
#include "he/macro.h"
#include "he/memory.h"
#include "he/err.h"
#include "he/he.h"
#include "he/vec.h"
#include "he/tri.h"
#include "he/y.h"
#include "he/util.h"
#include "he/strain.h"
#include "he/strain/3d.h"

#include "inc/def.h"

#include "he/f/strain.h"

#define T HeFStrain
#define OUT HE_REAL_OUT

#define BEGIN                                                   \
for (t = 0; t < nt; t++) {                                      \
    he_tri_ijk(he, t, /**/ &i, &j, &k);                         \
    get3(q->x, q->y, q->z, i, j, k, /**/ a0, b0, c0);           \
    get3(x, y, z, i, j, /**/ k, a, b, c);
#define END }

static const real EPS = 1e-8;

struct T {
    He *he;
    real *x, *y, *z, *eng, *I1, *I2;
    real *I1t, *I2t, *engt; /* on triangles */
    Strain *strain;
};

static void zero(int n, real *a) {
    int i;
    for (i = 0; i < n; i++)
        a[i] = 0;
}

static int get3(const real *x, const real *y, const real *z,
                int i, int j, int k,  /**/
                real a[3], real b[3], real c[3]) {
    vec_get(i, x, y, z, /**/ a);
    vec_get(j, x, y, z, /**/ b);
    vec_get(k, x, y, z, /**/ c);
    return HE_OK;
}

int he_f_strain_ini(const char *off, const char *name, StrainParam param, T **pq) {
    T *q;
    int nv, nt, status;
    He *he;
    real *x, *y, *z;

    MALLOC(1, &q);

    status = y_ini(off, &he, &x, &y, &z);
    if (status != HE_OK)
        ERR(HE_IO, "y_ini failed");

    nv = he_nv(he);
    nt = he_nt(he);

    MALLOC(nv, &q->eng);
    MALLOC(nv, &q->I1);
    MALLOC(nv, &q->I2);

    MALLOC(nt, &q->I1t);
    MALLOC(nt, &q->I2t);
    MALLOC(nt, &q->engt);

    strain_ini(name, param, &q->strain);

    q->he = he;
    q->x = x;
    q->y = y;
    q->z = z;

    *pq = q;
    return HE_OK;
}

int he_f_strain_argv(char ***p, He *he, T **pq) {
#   define PAR(f) \
    do {                                                \
    if ((status = argv_real(p, &param.f)) != HE_OK)     \
        return status;                                  \
    } while (0)

    int status;
    StrainParam param;
    char off[MAX_STRING_SIZE], name[MAX_STRING_SIZE];

    if ((status = argv_str(p, off)) != HE_OK)
        return status;

    if ((status = argv_str(p, name)) != HE_OK)
        return status;

    if (util_eq(name, "linear")) {
        PAR(Ka);
        PAR(mu);
    } else if (util_eq(name, "lim")) {
        PAR(Ka);
        PAR(mu);
        PAR(a3);
        PAR(a4);
        PAR(b1);
        PAR(b2);
    } else
        ERR(HE_IO, "unknown strain model '%s'", name);

    status = he_f_strain_ini(off, name, param, pq);
    return status;
#   undef PAR
}

int he_f_strain_fin(T *q) {
    y_fin(q->he, q->x, q->y, q->z);
    strain_fin(q->strain);
    FREE(q->eng); FREE(q->I1); FREE(q->I2);
    FREE(q->I1t); FREE(q->I2t); FREE(q->engt);
    FREE(q);
    return HE_OK;
}

static int small(const real a[3]) { return vec_abs(a) < EPS; }
static int assert_force(const real a[3], const real b[3], const real c[3],
                        const real da[3], const real db[3], const real dc[3]) {
    /* check force and  torque */
    real m[3], f[3], t[3], ma[3], mb[3], mc[3];
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
        vec_fprintf(a, stderr, OUT);
        vec_fprintf(b, stderr, OUT);
        vec_fprintf(c, stderr, OUT);
        vec_fprintf(f, stderr, OUT);
        vec_fprintf(t, stderr, OUT);
        return 0;
    } else
        return 1;
}

int he_f_strain_force(T *q, __UNUSED He* he0, const real *x, const real *y, const real *z, /**/
                            real *fx, real *fy, real *fz) {
    const real *x0, *y0, *z0;
    real a0[3], b0[3], c0[3];
    real a[3], b[3], c[3], da[3], db[3], dc[3];
    int nv, nt, t;
    int i, j, k;
    He *he;
    he = q->he; nv = he_nv(he); nt = he_nt(he);
    
    x0 = q->x; y0 = q->y; z0 = q->z;
    if (nv != he_nv(he0))
        ERR(HE_INDEX, "nv=%d != he_nv(he0)=%d", nv, he_nv(he0));
    if (nt != he_nt(he0))
        ERR(HE_INDEX, "nt=%d != he_nt(he0)=%d", nt, he_nt(he0));

    BEGIN {
        if (!assert_force(a, b, c, da, db, dc))
            ERR(HE_NUM,
                "bad forces in triangle: %d [%d %d %d]", t, i, j, k);
        vec_append(da, i, /**/ fx, fy, fz);
        vec_append(db, j, /**/ fx, fy, fz);
        vec_append(dc, k, /**/ fx, fy, fz);
    } END;
    return HE_OK;
}

real he_f_strain_energy(T *q, He* he0, const real *x, const real *y, const real *z) {
    He *he;
    real a0[3], b0[3], c0[3];
    real a[3], b[3], c[3];
    int nv, nt, t;
    int i, j, k;
    real e0, e, *eng, *engt;
    int *t0, *t1, *t2;

    if (nv != he_nv(he0))
        ERR(HE_INDEX, "nv=%d != he_nv(he0)=%d", nv, he_nv(he0));
    if (nt != he_nt(he0))
        ERR(HE_INDEX, "nt=%d != he_nt(he0)=%d", nt, he_nt(he0));

    e = 0;
    eng = q->eng;
    engt = q->engt;

    zero(nv, eng);
    BEGIN {
        e0 = strain_energy(q->strain, a0, b0, c0, a, b, c);
        engt[t] = e0;
        eng[i] += e0/3; eng[j] += e0/3; eng[k] += e0/3;
        e += e0;
    } END;
    return e;
}

int he_f_strain_invariants(T *q, const real *x, const real *y, const real *z, /**/ real **pI1, real **pI2) {
    real a0[3], b0[3], c0[3];
    real a[3], b[3], c[3];
    int nt, nv, t;
    int i, j, k;
    real *I1, *I2, I10, I20;
    He *he;
    he = q->he; nv = he_nv(he); nt = he_nt(he);

    I1 = q->I1;
    I2 = q->I2;
    zero(nv, I1);
    zero(nv, I2);

    BEGIN {
        strain_invariants(a0, b0, c0, a, b, c, &I10, &I20);
        I1[i] += I10/3; I1[j] += I10/3; I1[k] += I10/3;
        I2[i] += I10/3; I2[j] += I20/3; I2[k] += I20/3;
    } END;

    *pI1 = I1;
    *pI2 = I2;

    return HE_OK;
}

int he_f_strain_invariants_tri(T *q, const real *x, const real *y, const real *z, /**/ real **pI1, real **pI2) {
    real a0[3], b0[3], c0[3];
    real a[3], b[3], c[3];
    int nt, t;
    int i, j, k;
    real *I1t, *I2t, I10, I20;
    He *he;
    he = q->he; nt = he_nt(he);

    I1t = q->I1t;
    I2t = q->I2t;

    BEGIN {
        strain_invariants(a0, b0, c0, a, b, c, &I10, &I20);
        I1t[t] = I10;
        I2t[t] = I20;
    } END;

    *pI1 = I1t;
    *pI2 = I2t;

    return HE_OK;
}

int he_f_strain_energy_ver(T *q, /**/ real **pa) {
    *pa = q->eng;
    return HE_OK;
}

int he_f_strain_energy_tri(T *q, /**/ real **pa) {
    *pa = q->engt;
    return HE_OK;
}
