#include <stdio.h>

#include <real.h>
#include <co/err.h>
#include <co/he.h>
#include <co/memory.h>
#include <co/off.h>
#include <co/y.h>

int
main()
{
    real *x, *y, *z, *rank;
    He *he;
    int nv, i, n, status;

    err_set_ignore();
    y_inif(stdin, &he, &x, &y, &z);
    nv = he_nv(he);
    MALLOC(nv, &rank);
    for (i = 0; i < nv; i++) {
	if (he_bnd_ver(he, i))
	    rank[i] = 0;
	else {
	    status = he_rank(he, i, &n);
	    if (status != CO_OK)
		ER("he_rank failed for i = %d", i);
	    rank[i] = n;
	}
    }
    boff_ver_fwrite(he, x, y, z, rank, stdout);
    FREE(rank);
    return y_fin(he, x, y, z);
}
