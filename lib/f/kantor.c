#include <stdio.h>

#include "real.h"
#include "he/memory.h"
#include "he/macro.h"
#include "he/err.h"
#include "he/he.h"
#include "he/vec.h"
#include "he/dih.h"
#include "he/ddih.h"

#include "he/f/kantor.h"

#define T HeFKantor

struct T {
  int ne;
  real *kcos, *energy;
  real Kb, theta0;
};

static real compute_energy(He *he, const real *kcos, /**/ real *energy) {
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
    
    if (he_bnd(he, h)) continue;
    e0 = 1 - kcos[m];
    
    energy[i] += e0/2;
    energy[j] += e0/2;

    e += e0;
  }
  return e;
}
int he_f_kantor_ini(real Kb, real theta0, He *he, T **pq) {
    T *q;
    int ne, nv;
    MALLOC(1, &q);
    ne = he_ne(he);
    nv = he_nv(he);

    MALLOC(ne, &q->kcos);
    MALLOC(nv, &q->energy);

    q->ne = ne;
    q->Kb = Kb;
    q->theta0 = theta0;

    *pq = q;
    return HE_OK;
}

int he_f_kantor_fin(T *q) {
    FREE(q->kcos); FREE(q->energy); FREE(q);
    return HE_OK;
}

int he_f_kantor_cos(T *q, /**/ real  **pa) {
    *pa = q->kcos;
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


static void compute_cos(He *he, const real *x, const real *y, const real *z, /**/ real *kcos) {
    real a[3], b[3], c[3], d[3];
    int status, n, m;
    n = he_ne(he);
    for (m = 0; m < n; m++) {
        status = get(m, he, x, y, z, /**/ a, b, c, d);
        if (status == BND) continue;
	kcos[m] = dih_cos_sup(a, b, c, d);
    }
}

static void compute_force(real Kb,
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
        ddih_cos_sup(a, b, c, d, /**/ da, db, dc, dd);
        vec_scalar_append(da, 2*Kb, i, /**/ fx, fy, fz);
        vec_scalar_append(db, 2*Kb, j, /**/ fx, fy, fz);
        vec_scalar_append(dc, 2*Kb, k, /**/ fx, fy, fz);
        vec_scalar_append(dd, 2*Kb, l, /**/ fx, fy, fz);
    }
}

int he_f_kantor_force(T *q, He *he,
                      const real *x, const real *y, const real *z, /**/
                      real *fx, real *fy, real *fz) {
    int ne;
    real *kcos, Kb;
    
    ne  = q->ne;
    kcos = q->kcos;
    Kb   = q->Kb;
    
    if (he_ne(he) != ne)
      ERR(HE_INDEX, "he_ne(he)=%d != n = %d", he_ne(he), ne);
    compute_cos(he, x, y, z, /**/ kcos);
    compute_force(Kb, he, x, y, z, /**/ fx, fy, fz);
    
    return HE_OK;
}

real he_f_kantor_energy(T *q, He *he,
                      const real *x, const real *y, const real *z) {
    int ne;
    real *kcos, *energy, Kb;
    
    ne   = q->ne;
    kcos = q->kcos;
    energy = q->energy;
    Kb  = q->Kb;

    if (he_ne(he) != ne)
        ERR(HE_INDEX, "he_ne(he)=%d != n = %d", he_ne(he), ne);
    compute_cos(he, x, y, z, /**/ kcos);
    return 2*Kb*compute_energy(he, kcos, /**/ energy);
}


int he_f_kantor_energy_ver(T *q, /**/ real**pa) {
    *pa = q->energy;
    return HE_OK;
}
