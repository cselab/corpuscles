#include <stdio.h>
#include <stdlib.h>
#include <real.h>
#include <co/err.h>
#include <co/he.h>
#include <co/he/invariant.h>
#include <co/macro.h>
#include <co/remesh.h>
#include <co/memory.h>
#include <co/povray.h>
#include <co/vec.h>
#include <co/tri.h>
#include <co/y.h>

const char *me = "povray/mesh2";
static void
usg()
{
    fprintf(stderr, "%s < OFF > POV\n", me);
    exit(2);
}

int
main(int __UNUSED argc, const char **argv)
{
    enum {X, Y, Z};
    He *he;
    real *x, *y, *z, *tri;
    int nt, i;

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
    nt = he_nt(he);
    MALLOC(nt, &tri);

    int u, v, w;
    real a[3], b[3], c[3], r[3];
    for (i = 0; i < nt; i++) {
      he_tri_ijk(he, i, &u, &v, &w);
      vec_get3(u, v, w, x, y, z, a, b, c);
      tri_center(a, b, c, r);
      tri[i] = r[X];
    }
    
    povray_tri_mesh2(he, x, y, z, tri, stdout);
    y_fin(he, x, y, z);
    FREE(tri);
}
