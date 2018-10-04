#include <math.h>
#include <stdio.h>

#include "real.h"
#include "inc/def.h"
#include "he/err.h"
#include "he/vec.h"
#include "he/he.h"
#include "he/tri.h"
#include "he/equiangulate.h"

static const real pi = 3.141592653589793;

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

static int get(He *he, const real *x, const real *y, const real *z, int e,
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

static int good(He *he, real *x, real *y, real *z, int e) {
    real al, be;
    real a[3], b[3], c[3], d[3];
    get(he, x, y, z, e, /**/ a, b, c, d);
    al = tri_angle(c, a, b);
    be = tri_angle(b, d, c);
    return al + be < pi;
}

static int equiangulate0(He *he, real *x, real *y, real *z, int e) {
    he_edg_rotate(he, e);
}

int he_equiangulate(He *he, real *x, real *y, real *z, /**/ int *pcnt) {
    int e, ne, cnt;
    ne = he_ne(he);
    cnt = 0;
    for (e = 0; e < ne; e++) {
        if (!good(he, x, y, z, e)) {
            equiangulate0(he, x, y, z, e);
            cnt++;
        }
    }
    *pcnt = cnt;
    return HE_OK;
}
