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

static const char *me = "del/7";
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
    real *x, *y, *z, *color;
    int i, nt, nv;
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
	if (rank == 7)
	    break;
    }
    if (i == nv)
	ER("i=%d == nv=%d", i, nv);
    MSG("nv: %d", he_nv(he));
    MSG("nt: %d", he_nt(he));
    if (he_ver_delete7(he, i) != CO_OK)
	ER("he_ver_delete7 failed");
    if (he_invariant(he) != CO_OK)
      ER("he_invariant failed");
    vec_swap(i, nv - 1, x, y, z);
    nt = he_nt(he);
    CALLOC(nt, &color);
    MSG("nv: %d", he_nv(he));
    MSG("nt: %d", he_nt(he));

    int t;
    t = he_tri(he, he_hdg_ver(he, ring[1]));
    MSG("t: %d", t);
    color[t] = 1;
    
    boff_tri_fwrite(he, x, y, z, color, stdout);
    FREE(color);
    y_fin(he, x, y, z);
}
