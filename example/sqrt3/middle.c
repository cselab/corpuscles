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

static const char *me = "sqrt3/middle";
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
    for (i = 0; i < nv; i++) {
	
	he_ring(he, i, &nring, &ring);
	for (
	

	
        vec_get(i, x, y, z, a);
        vec_set(a, i, x0, y0, z0);
    }

    for (e = 0; e < ne; e++) {
        he_edg_ij(he, e, &i, &j);
        vec_get2(i, j, x, y, z, a, b);
        edg_center(a, b, ab);
        ij = nv + e;
        vec_set(ab, ij, x0, y0, z0);
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
