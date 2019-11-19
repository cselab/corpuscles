#include <stdio.h>
#include <stdlib.h>
#include <real.h>
#include <co/err.h>
#include <co/he.h>
#include <co/he/invariant.h>
#include <co/macro.h>
#include <co/memory.h>
#include <co/off.h>
#include <co/vec.h>
#include <co/tri.h>
#include <co/y.h>

const char *me = "del/5";
static void
usg()
{
    fprintf(stderr, "%s < OFF\n", me);
    exit(2);
}

int
main(int __UNUSED argc, const char **argv)
{
    He *he;
    real *x, *y, *z, *color;
    const char *arg;
    int t, i, j, k, nt, nv, v, status;
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

    int rank, *ring;
    for (i = 0; i < nv; i++) {
	he_ring(he, i, &rank, &ring);
	if (rank == 5)
	    break;
    }
    status = he_ver_delete5(he, v);
    if (status != CO_OK)
	ER("he_ver_delete5 failed");
    status = he_invariant(he);
    if (status != CO_OK)
      ER("he_invariant failed");
    nt = he_nt(he);
    CALLOC(nt, &color);
    boff_tri_fwrite(he, x, y, z, color, stdout);
    FREE(color);
    y_fin(he, x, y, z);
}
