#include <stdio.h>

#include <real.h>
#include <co/err.h>
#include <co/he.h>
#include <co/memory.h>
#include <co/vtk.h>
#include <co/y.h>

int
main()
{
    real *x, *y, *z;
    He *he;
    int nv, nt, i, j, n, *ring, status, *rank;

    err_set_ignore();

    y_inif(stdin, &he, &x, &y, &z);
    nv = he_nv(he);
    MALLOC(nv, &rank);

    for (i = 0; i < nv; i++) {
        if (he_bnd_ver(he, i))
            rank[i] = 0;
        else {
            status = he_ring(he, i, &n, &ring);
            if (status != CO_OK)
                ER("he_ring failed for i = %d", i);
            rank[i] = n;
        }
    }
    for (i = 0; i < nv; i++)
        printf("%d\n", rank[i]);
    FREE(rank);
    y_fin(he, x, y, z);
}
