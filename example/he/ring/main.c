#include <stdio.h>

#include <real.h>
#include <he/err.h>
#include <he/he.h>
#include <he/y.h>

int main() {
    real *x, *y, *z;
    He      *he;
    int nv, nt, i, n, *ring, status;

    err_set_ignore();

    y_inif(stdin, &he, &x, &y, &z);
    nv = he_nv(he);
    for (i = 0; i < nv; i++) {
        if (he_bnd_ver(he, i)) continue;
        status = he_ring(he, i, &n, &ring);
        if (status != HE_OK)
            ER("he_ring failed for i = %d", i);
        printf("%d %d\n", i, n);
    }
    return y_fin(he, x, y, z);
}
