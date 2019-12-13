#include <math.h>
#include <stdio.h>

#include "real.h"
#include "co/err.h"
#include "co/he.h"
#include "co/vec.h"
#include "co/tri.h"
#include "co/remesh.h"
#include "co/memory.h"

static int tri_near(He * he, int t, int *i, int *j, int *k);
static int t2edg(He * he, int t, int *, int *, int *);
static int t2ver(He * he, int t, int *, int *, int *);

int
remesh_loop(He * he, real ** px, real ** py, real ** pz, He ** phe0)
{
    He *he0;
    int e;
    int eij;
    int ejk;
    int eki;
    int i;
    int i0;
    int ij;
    int j;
    int jk;
    int k;
    int ki;
    int l;
    int ne;
    int nring;
    int nt;
    int nt0;
    int nv;
    int nv0;
    int *ring;
    int t;
    int *tri;
    real a[3];
    real ab[3];
    real alpha;
    real b[3];
    real bc[3];
    real beta;
    real c[3];
    real ca[3];
    real d[3];
    real g[3];
    real *x;
    real *x0;
    real *y;
    real *y0;
    real *z;
    real *z0;

    x = *px;
    y = *py;
    z = *pz;
    ne = he_ne(he);
    nv = he_nv(he);
    nt = he_nt(he);
    nv0 = nv + ne;
    nt0 = 4 * nt;
    MALLOC(nv0, &x0);
    MALLOC(nv0, &y0);
    MALLOC(nv0, &z0);
    MALLOC(3 * nt0, &tri);
#define ADD(i, j, k) tri[i0++] = (i), tri[i0++] = (j), tri[i0++] = (k)
    for (i0 = t = 0; t < nt; t++) {
        t2ver(he, t, &i, &j, &k);
        t2edg(he, t, &eij, &ejk, &eki);
        ij = nv + eij;
        jk = nv + ejk;
        ki = nv + eki;
        ADD(i, ij, ki);
        ADD(ij, j, jk);
        ADD(ij, jk, ki);
        ADD(ki, jk, k);
        vec_get3(i, j, k, x, y, z, a, b, c);
        tri_edg_center(a, b, c, bc, ca, ab);
    }
    if (i0 != 3 * nt0)
        ERR(CO_INDEX, "i0=%d != nt0=%d", i0, nt0);

    for (i = 0; i < nv; i++) {
        if (he_bnd_ver(he, i))
            ERR(CO_INDEX, "i=%d is on the boundary", i);
        vec_get(i, x, y, z, a);
        he_ring(he, i, &nring, &ring);
        vec_zero(b);
        for (j = 0; j < nring; j++) {
            vec_get(ring[j], x, y, z, c);
            vec_add(c, b);
        }
        beta = nring == 3 ? 3.0 / 16 : 3.0 / (8 * nring);
        alpha = 1 - nring * beta;
        vec_linear_combination(alpha, a, beta, b, /**/ c);
        vec_set(c, i, x0, y0, z0);
    }

    for (e = 0; e < ne; e++) {
        he_dih_ijkl(he, e, &i, &j, &k, &l);
        vec_get4(i, j, k, l, x, y, z, a, b, c, d);
        vec_zero(g);
        vec_axpy(3.0 / 8, b, g);
        vec_axpy(3.0 / 8, c, g);
        vec_axpy(1.0 / 8, a, g);
        vec_axpy(1.0 / 8, d, g);
        ij = nv + e;
        vec_set(g, ij, x0, y0, z0);
    }
    if (he_tri_ini(nv0, nt0, tri, &he0) != CO_OK)
        ERR(CO_INDEX, "he_tri_ini failed");

    FREE3(x, y, z);
    FREE(tri);
    *phe0 = he0;
    *px = x0;
    *py = y0;
    *pz = z0;
    return CO_OK;
}

int
remesh_tri_split(int nmax, int *tri, int n, He * he, real ** px,
                 real ** py, real ** pz)
{
    int a;
    int b;
    int c;
    int i;
    int ia;
    int ib;
    int ic;
    int j;
    int *killed;
    int nt;
    int nv;
    int t;
    real u[3];
    real v[3];
    real va[3];
    real vb[3];
    real vc[3];
    real w[3];
    real *x;
    real *y;
    real *z;

    x = *px;
    y = *py;
    z = *pz;
    nt = he_nt(he);
    i = j = 0;
    CALLOC(nt, &killed);
    while (i < nmax && j < n) {
        t = tri[i];
        if (t < 0 || t >= nt)
            ERR(CO_INDEX, "tri[%d]=%d is not in [0, %d)", i, t, nt);
        if (!killed[t] && tri_near(he, t, &a, &b, &c) == 0) {
            he_tri_ijk(he, t, &ia, &ib, &ic);
            vec_get3(ia, ib, ic, x, y, z, va, vb, vc);
            if (he_tri_split(he, t) != CO_OK)
                ERR(CO_INDEX, "he_tri_split failed, t = %d", t);
            if (a < nt)
                killed[a] = 1;
            if (b < nt)
                killed[b] = 1;
            if (c < nt)
                killed[c] = 1;
            nv = he_nv(he);
            REALLOC(nv, &x);    /* TODO */
            REALLOC(nv, &y);
            REALLOC(nv, &z);
            tri_edg_center(va, vb, vc, u, v, w);
            vec_set(v, nv - 1, x, y, z);
            vec_set(u, nv - 2, x, y, z);
            vec_set(w, nv - 3, x, y, z);
            j++;
        }
        i++;
    }
    *px = x;
    *py = y;
    *pz = z;
    FREE(killed);
    return CO_OK;
}

static int
tri_near(He * he, int t, int *pi, int *pj, int *pk)
{
    int i;
    int j;
    int k;

    i = he_hdg_tri(he, t);
    j = he_nxt(he, i);
    k = he_nxt(he, j);
    if (he_bnd(he, i) || he_bnd(he, j) || he_bnd(he, k))
        return 1;
    i = he_flp(he, i);
    j = he_flp(he, j);
    k = he_flp(he, k);
    *pi = he_tri(he, i);
    *pj = he_tri(he, j);
    *pk = he_tri(he, k);
    return 0;
}

static int
t2edg(He * he, int t, int *i, int *j, int *k)
{
    int h, n, nn;

    h = he_hdg_tri(he, t);
    n = he_nxt(he, h);
    nn = he_nxt(he, n);
    *i = he_edg(he, h);
    *j = he_edg(he, n);
    *k = he_edg(he, nn);

    return CO_OK;
}

static int
t2ver(He * he, int t, int *i, int *j, int *k)
{
    int h, n, nn;

    h = he_hdg_tri(he, t);
    n = he_nxt(he, h);
    nn = he_nxt(he, n);
    *i = he_ver(he, h);
    *j = he_ver(he, n);
    *k = he_ver(he, nn);

    return CO_OK;
}
