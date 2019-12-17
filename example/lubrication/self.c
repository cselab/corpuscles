#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <real.h>
#include <co/edg.h>
#include <co/err.h>
#include <co/he.h>
#include <co/macro.h>
#include <co/memory.h>
#include <co/off.h>
#include <co/surface.h>
#include <co/vec.h>
#include <co/y.h>

static const char *me = "lubrication/self";
#define FMT   CO_REAL_OUT

static void
usg()
{
    fprintf(stderr, "%s [-p] OFF OFF > OFF\n", me);
    exit(2);
}
static real dist_ij(int, int, const real *, const real *, const real *);

int
main(int argc, char **argv)
{
    He *he;
    int i;
    int InRing;
    int j;
    int k;
    int nring;
    int nv;
    int nt;
    int Punto;
    int *ring;
    real d;
    real *distance;
    real *distance_tri;
    real dm;
    real *x;
    real *y;
    real *z;

    USED(argc);
    Punto = 0;
    while (*++argv != NULL && argv[0][0] == '-')
        switch (argv[0][1]) {
        case 'h':
            usg();
            break;
	case 'p':
	    Punto = 1;
	    break;
        default:
            fprintf(stderr, "%s: unknown option '%s'\n", me, argv[0]);
            exit(2);
        }
    y_inif(stdin, &he, &x, &y, &z);
    nv = he_nv(he);
    nt = he_nt(he);
    MALLOC(nv, &distance);
    for (i = 0; i < nv; i++) {
        dm = 1e32;
        he_ring(he, i, &nring, &ring);
        for (j = 0; j < nv; j++) {
            if (j == i)
                continue;
            InRing = 0;
            for (k = 0; k < nring; k++)
                if (ring[k] == j)
                    InRing = 1;
            if (InRing)
                continue;
            d = dist_ij(i, j, x, y, z);
            if (d < dm)
                dm = d;
        }
        distance[i] = 1/(dm*dm);
    }
    surface_ver2tri(he, distance, &distance_tri);
    if (Punto) {
	for (i = 0; i < nt; i++)
	    printf(FMT "\n", distance_tri[i]);
    } else {
	boff_tri_fwrite(he, x, y, z, distance_tri, stdout);
    }
    
    y_fin(he, x, y, z);
    FREE(distance);
    FREE(distance_tri);
}

static real
dist_ij(int i, int j, const real * x, const real * y, const real * z)
{
    real a[3];
    real b[3];

    vec_get2(i, j, x, y, z, a, b);
    return edg_abs(a, b);
}
