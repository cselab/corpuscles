#include <stdio.h>
#include <stdlib.h>
#include <real.h>
#include <co/area.h>
#include <co/err.h>
#include <co/he.h>
#include <co/macro.h>
#include <co/memory.h>
#include <co/off.h>
#include <co/vec.h>
#include <co/tri.h>
#include <co/y.h>

static const char *me = "split/big";
static void
usg(void)
{
    fprintf(stderr, "%s < OFF\n", me);
    exit(2);
}

int
main(int __UNUSED argc, const char **argv)
{
    He *he;
    real *x;
    real *y;
    real *z;
    real *area;
    real a[3];
    real b[3];
    real c[3];
    real d[3];
    int nt;
    int nv;
    int t;
    int i;
    int j;
    int k;
    int Verbose;
    Verbose = 0;
    while (*++argv != NULL && argv[0][0] == '-')
        switch (argv[0][1]) {
        case 'h':
            usg();
            break;
        case 'v':
            Verbose = 1;
            break;
        default:
            fprintf(stderr, "%s: unknown option '%s'\n", me, argv[0]);
            exit(2);
        }
    y_inif(stdin, &he, &x, &y, &z);
    nt = he_nt(he);
    MALLOC(nt, &area);
    he_area_tri(he, x, y, z, /**/ area);
    t = 0;
    for (i = 0; i < nt; i++)
      if (area[i] > area[t])
        t = i;
    if (Verbose)
      fprintf(stderr, "t = %d\n", t);
    he_tri_ijk(he, t, &i, &j, &k);
    vec_get3(i, j, k, x, y, z, a, b, c);
    tri_center(a, b, c, /**/ d);
    if (he_tri_split3(he, t) != CO_OK)
        ER("he_tri_split failed");
    nv = he_nv(he);
    REALLOC(nv, &x);
    REALLOC(nv, &y);
    REALLOC(nv, &z);
    vec_set(d, nv - 1, x, y, z);
    if (off_he_xyz_fwrite(he, x, y, z, stdout) != CO_OK)
      ER("fail to write off file");
    y_fin(he, x, y, z);
    FREE(area);
}
