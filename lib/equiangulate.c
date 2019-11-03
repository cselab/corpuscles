#include <tgmath.h>
#include <stdio.h>

#include "real.h"
#include "inc/def.h"
#include "co/err.h"
#include "co/vec.h"
#include "co/he.h"
#include "co/tri.h"
#include "co/equiangulate.h"

static const real pi = 3.141592653589793;

#define  nxt(h)     he_nxt(he, h)
#define  flp(h)     he_flp(he, h)
#define  ver(h)     he_ver(he, h)
#define  hdg_ver(v) he_hdg_ver(he, v)
#define  hdg_edg(e) he_hdg_edg(he, e)
#define  bnd(h)     he_bnd(he, h)

enum { BULK, BND };
static int
get_ijkl(int e, He * he, /**/ int *pi, int *pj, int *pk, int *pl)
{
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
get(He * he, const real * x, const real * y, const real * z, int e,
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
good(He * he, const real * x, const real * y, const real * z, int e)
{
    real al, be;
    real a[3], b[3], c[3], d[3];

    get(he, x, y, z, e, /**/ a, b, c, d);
    al = tri_angle(c, a, b);
    be = tri_angle(b, d, c);
    return al + be < pi;
}

static int
ear(He * he, int v, int v_new)
{
    int h0, h, f, vc;

    h0 = h = hdg_ver(v);
    do {
        if (bnd(h))
            return 0;
        f = flp(h);
        vc = ver(f);
        if (vc == v_new)
            return 1;
        h = nxt(flp(h));
    } while (h != h0);
    return 0;
}

int
he_ear(He * he, int e0)
{
    int h0, h2, h5, v2, v3;

    h0 = hdg_edg(e0);
    h2 = nxt(nxt(h0));
    h5 = nxt(nxt(flp(h0)));

    v2 = ver(h2);
    v3 = ver(h5);
    return ear(he, v2, v3);
}

static int
equiangulate0(He * he, int e)
{
    return he_edg_rotate(he, e);
}

int
he_equiangulate(He * he, const real * x, const real * y, const real * z,
                /**/ int *pcnt)
{
    int e, ne, cnt;

    ne = he_ne(he);
    cnt = 0;
    for (e = 0; e < ne; e++) {
        if (he_ear(he, e))
            continue;
        if (!good(he, x, y, z, e)) {
            equiangulate0(he, e);
            cnt++;
        }
    }
    *pcnt = cnt;
    return CO_OK;
}
