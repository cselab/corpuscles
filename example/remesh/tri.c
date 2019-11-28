#include <stdio.h>
#include <stdlib.h>
#include <real.h>
#include <co/err.h>
#include <co/he.h>
#include <co/he/invariant.h>
#include <co/macro.h>
#include <co/remesh.h>
#include <co/memory.h>
#include <co/off.h>
#include <co/vec.h>
#include <co/tri.h>
#include <co/y.h>

const char *me = "remesh/tri";
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
    real *x, *y, *z;

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

    int n, nmax;
    int tri[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    nmax = n = 10;
    remesh_tri_split(nmax, tri, n, he, &x, &y, &z);
    if (he_invariant(he) != 0)
	ER("he_invariant failed");
    boff_fwrite(he, x, y, z, stdout);
    y_fin(he, x, y, z);
}
