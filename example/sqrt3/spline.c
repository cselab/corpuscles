#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <real.h>
#include <co/err.h>
#include <co/he.h>
#include <co/macro.h>
#include <co/memory.h>
#include <co/off.h>
#include <co/vec.h>
#include <co/edg.h>
#include <co/tri.h>
#include <co/y.h>

static const char *me = "sqrt3/spline";
static void
usg()
{
    fprintf(stderr, "%s OFF > OFF\n", me);
    exit(2);
}

static int t2edg(He * he, int t, int *, int *, int *);
static int t2ver(He * he, int t, int *, int *, int *);

int
main(int argc, char **argv)
{
    USED(argc);
    He *he;
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
    int ne;
    int nt;
    int nt0;
    int nv;
    int nv0;
    int t;
    int *tri;
    real a[3];
    real ab[3];
    real b[3];
    real bc[3];
    real c[3];
    real ca[3];
    real *x;
    real *x0;
    real *y;
    real *y0;
    real *z;
    real *z0;
    int nring;
    int *ring;
    while (*++argv != NULL && argv[0][0] == '-')
        switch (argv[0][1]) {
        case 'h':
            usg();
            break;
        default:
            ER("unknown option '%s'", argv[0]);
        }
    y_inif(stdin, &he, &x, &y, &z);
    ne = he_ne(he);
    nv = he_nv(he);
    nt = he_nt(he);
    nv0 = nv + nt;
    nt0 = 3 * nt;
    MALLOC(nv0, &x0);
    MALLOC(nv0, &y0);
    MALLOC(nv0, &z0);
    MALLOC(3 * nt0, &tri);
#define ADD(i, j, k) tri[i0++] = (i), tri[i0++] = (j), tri[i0++] = (k)
    int v;
    int v0;
    int iring;
    real g[3];
    i0 = 0;
    v0 = nv;
    for (v = 0; v < nv; v++) {
	he_tri_ring(he, v, &nring, &ring);
	for (iring = 0; ring[iring + 1] != -1; iring++) {
	    he_tri_ijk(he, ring[iring], &i, &j, &k);
	    vec_get3(i, j, k, x, y, z, a, b, c);
	    tri_center(a, b, c, g);
	    vec_set(g, nv + ring[iring], x0, y0, z0);
	    ADD(v, nv + ring[iring], nv + ring[iring + 1]);
	}
	ADD(v, nv + ring[iring], nv + ring[0]);
    }
    for (i = 0; i < nv; i++) {
        vec_get(i, x, y, z, a);
        vec_set(a, i, x0, y0, z0);
    }

    he_tri_ini(nv0, nt0, tri, &he0);
    if (off_he_xyz_fwrite(he0, x0, y0, z0, stdout) != CO_OK)
        ER("off_he_xyz_fwrite failed");

    FREE(x0);
    FREE(y0);
    FREE(z0);
    FREE(tri);
    he_fin(he0);
    y_fin(he, x, y, z);
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
