#include <stdio.h>

#include <real.h>
#include <he/err.h>
#include <he/he.h>
#include <he/memory.h>
#include <he/vtk.h>
#include <he/y.h>

int main() {
    real *x, *y, *z, *rank;
    He      *he;
    int nv, nt, i, j, n, *ring, status;
    err_set_ignore();

    y_inif(stdin, &he, &x, &y, &z);
    nv = he_nv(he);
    MALLOC(nv, &rank);

    for (i = 0; i < nv; i++) {
        if (he_bnd_ver(he, i))
            rank[i] = 0;
        else  {
            status = he_ring(he, i, &n, &ring);
            if (status != HE_OK)
                ER("he_ring failed for i = %d", i);
            rank[i] = n;
        }
    }

    const char *na[] = {"rank",  NULL};
    const real *sc[] = {rank, NULL};
    vtk_fwrite(he, x, y, z, sc, na, stdout);
    FREE(rank);
    return y_fin(he, x, y, z);
}
