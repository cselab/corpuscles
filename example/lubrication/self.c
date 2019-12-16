#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <real.h>
#include <co/err.h>
#include <co/edg.h>
#include <co/he.h>
#include <co/macro.h>
#include <co/memory.h>
#include <co/off.h>
#include <co/vec.h>
#include <co/y.h>

static const char *me = "lubrication/self";
static void
usg()
{
    fprintf(stderr, "%s OFF OFF > OFF\n", me);
    exit(2);
}

static real dist_ij(int, int, const real *, const real *, const real *);

int
main(int argc, char **argv)
{
    He *he;
    real *x;
    real *y;
    real *z;
    real *distance;
    int *ring;
    int nring;
    int i;
    int j;
    int nv;
    real d;

    USED(argc);
    while (*++argv != NULL && argv[0][0] == '-')
        switch (argv[0][1]) {
        case 'h':
            usg();
            break;
        default:
            fprintf(stderr, "%s: unknown option '%s'\n", me, argv[0]);
            exit(2);
        }
    y_inif(stdin, &he, &x, &y, &z);
    nv = he_nv(he);
    MALLOC(nv, &distance);

    int k;
    int InRing;
    real dm;

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
        distance[i] = 1/(dm*dm*dm);
	MSG("%g", (double)dm);
    }
    boff_ver_fwrite(he, x, y, z, distance, stdout);
    y_fin(he, x, y, z);
    FREE(distance);
}

static real
dist_ij(int i, int j, const real *x, const real *y, const real *z)
{
  real a[3];
  real b[3];

  vec_get2(i, j, x, y, z, a, b);
  return edg_abs(a, b);
}
