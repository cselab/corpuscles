#include <stdio.h>
#include <tgmath.h>

#include "real.h"
#include "co/ddih.h"
#include "co/dih.h"
#include "co/err.h"
#include "co/f/kantor.h"
#include "co/he.h"
#include "co/macro.h"
#include "co/memory.h"
#include "co/tri.h"
#include "co/vec.h"


#define T HeFKantor

#define  nxt(h)     he_nxt(he, h)
#define  flp(h)     he_flp(he, h)
#define  ver(h)     he_ver(he, h)
#define  hdg_ver(v) he_hdg_ver(he, v)
#define  hdg_edg(e) he_hdg_edg(he, e)
#define  hdg_tri(t) he_hdg_tri(he, t)
#define  bnd(h)     he_bnd(he, h)

typedef struct Size Size;
struct Size {
    int nv, ne, nt;
};

struct T {
    Size size;
    real *energy, *theta;
    real *area;
    real Kb, theta0;
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
compute_energy(He * he, real Kb, real theta0, const real * theta,
               /**/ real * energy)
{
    int ne, nv, m, h, f;
    int i, j;
    real e, e0;

    ne = he_ne(he);
    nv = he_nv(he);

    for (m = 0; m < nv; m++)
        energy[m] = 0;

    e = 0;
    for (m = 0; m < ne; m++) {
        h = he_hdg_edg(he, m);
        f = he_flp(he, h);

        i = he_ver(he, h);
        j = he_ver(he, f);

        if (he_bnd(he, h))
            continue;
        e0 = 2.0 * Kb * (1 - cos(theta[m] - theta0));

        energy[i] += e0 / 2;
        energy[j] += e0 / 2;

        e += e0;
    }
    return e;
}

int
he_f_kantor_ini(real Kb, real theta0, He * he, T ** pq)
{

    T *q;
    int nv, ne, nt;
    Size size;

    MALLOC(1, &q);

    nv = he_nv(he);
    ne = he_ne(he);
    nt = he_nt(he);

    size.nv = nv;
    size.ne = ne;
    size.nt = nt;
    q->size = size;

    MALLOC(nv, &q->area);
    MALLOC(ne, &q->theta);
    MALLOC(nv, &q->energy);

    q->Kb = Kb;
    q->theta0 = theta0;

    *pq = q;
    return CO_OK;
}

int
he_f_kantor_argv(__UNUSED char ***p, __UNUSED He * he, __UNUSED T ** pq)
{
    return CO_OK;
}

int
he_f_kantor_fin(T * q)
{
    FREE(q->area);
    FREE(q->theta);
    FREE(q->energy);
    FREE(q);
    return CO_OK;
}

int
he_f_kantor_theta(T * q, /**/ real ** pa)
{
    *pa = q->theta;
    return CO_OK;
}

enum { BULK, BND };
static int
get_ijkl(int e, He * he, /**/ int *pi, int *pj, int *pk, int *pl)
{
#define  nxt(h)     he_nxt(he, h)
#define  flp(h)     he_flp(he, h)
#define  ver(h)     he_ver(he, h)
#define  hdg_ver(v) he_hdg_ver(he, v)
#define  hdg_edg(e) he_hdg_edg(he, e)
#define  bnd(h)     he_bnd(he, h)
    int h, n, nn, nnf, i, j, k, l;

    h = he_hdg_edg(he, e);
    if (bnd(h))
        return BND;

    h = hdg_edg(e);
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

static int
get(int e, He * he, const real * x, const real * y, const real * z,
    /**/ real a[3], real b[3], real c[3], real d[3])
{
    int status, i, j, k, l;

    status = get_ijkl(e, he, /**/ &i, &j, &k, &l);
    if (status == BND)
        return BND;
    vec_get(i, x, y, z, /**/ a);
    vec_get(j, x, y, z, /**/ b);
    vec_get(k, x, y, z, /**/ c);
    vec_get(l, x, y, z, /**/ d);
    return BULK;
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
        //printf("a, b,c, d: %f %f %f\n", a[0], b[0], c[0]);
        area0 = tri_area(a, b, c) / 3;
        area[i] += area0;
        area[j] += area0;
        area[k] += area0;
    }
    //printf("area[0], area0: %f, %f\n", area[0], area0);
    return CO_OK;
}

static void
compute_theta(He * he, const real * x, const real * y, const real * z,
              /**/ real * theta)
{
    real a[3], b[3], c[3], d[3];
    int status, n, m;

    n = he_ne(he);
    for (m = 0; m < n; m++) {
        status = get(m, he, x, y, z, /**/ a, b, c, d);
        if (status == BND)
            continue;
        theta[m] = dih_angle_sup(a, b, c, d);
    }
}

static void
compute_force(He * he, real Kb, real theta0,
              const real * x, const real * y, const real * z, /**/
              real * fx, real * fy, real * fz)
{
    int status, n, t, i, j, k, l;
    real a[3], b[3], c[3], d[3], da[3], db[3], dc[3], dd[3];
    real theta, coef;

    n = he_ne(he);
    for (t = 0; t < n; t++) {
        status = get_ijkl(t, he, /**/ &i, &j, &k, &l);
        if (status == BND)
            continue;
        vec_get(i, x, y, z, /**/ a);
        vec_get(j, x, y, z, /**/ b);
        vec_get(k, x, y, z, /**/ c);
        vec_get(l, x, y, z, /**/ d);
        theta = dih_angle_sup(a, b, c, d);
        coef = 2.0 * Kb * sin(theta - theta0);
        ddih_angle_sup(a, b, c, d, /**/ da, db, dc, dd);
        vec_scalar_append(da, coef, i, /**/ fx, fy, fz);
        vec_scalar_append(db, coef, j, /**/ fx, fy, fz);
        vec_scalar_append(dc, coef, k, /**/ fx, fy, fz);
        vec_scalar_append(dd, coef, l, /**/ fx, fy, fz);
    }
}

int
he_f_kantor_force(T * q, He * he,
                  const real * x, const real * y, const real * z, /**/
                  real * fx, real * fy, real * fz)
{
    int ne;
    real *theta;
    real *area;
    real Kb, theta0;
    Size size;

    size = q->size;
    ne = size.ne;
    theta = q->theta;
    area = q->area;
    Kb = q->Kb;
    theta0 = q->theta0;

    if (he_ne(he) != ne)
        ERR(CO_INDEX, "he_ne(he)=%d != n = %d", he_ne(he), ne);
    compute_theta(he, x, y, z, /**/ theta);
    compute_force(he, Kb, theta0, x, y, z, /**/ fx, fy, fz);

    compute_area(he, size, x, y, z, /**/ area);

    return CO_OK;
}

real
he_f_kantor_energy(T * q, He * he,
                   const real * x, const real * y, const real * z)
{
    int ne;
    real *theta, *energy;
    real *area;
    real Kb, theta0;
    Size size;

    size = q->size;
    ne = size.ne;
    theta = q->theta;
    energy = q->energy;
    area = q->area;
    Kb = q->Kb;
    theta0 = q->theta0;

    if (he_ne(he) != ne)
        ERR(CO_INDEX, "he_ne(he)=%d != n = %d", he_ne(he), ne);
    compute_theta(he, x, y, z, /**/ theta);

    compute_area(he, size, x, y, z, /**/ area);

    return compute_energy(he, Kb, theta0, theta, /**/ energy);
}

int
he_f_kantor_energy_ver(T * q, /**/ real ** pa)
{
    *pa = q->energy;
    return CO_OK;
}

int
he_f_kantor_area_ver(T * q, /**/ real ** pa)
{
    *pa = q->area;
    return CO_OK;
}

int
he_f_kantor_curva_mean_ver(__UNUSED T * q, /**/ __UNUSED real ** pa)
{
    return CO_INDEX;
}

real
he_f_kantor_energy_ad(T * q)
{
    return q->eng_ad;
}

real
he_f_kantor_energy_bend(T * q)
{
    return q->eng_bend;
}
