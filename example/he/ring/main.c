#include <stdio.h>

#include <real.h>
#include <co/err.h>
#include <co/he.h>
#include <co/y.h>

int main() {
    real *x, *y, *z;
    He      *he;
    int nv, nt, i, j, n, *ring, status;

    err_set_ignore();

    y_inif(stdin, &he, &x, &y, &z);
    nv = he_nv(he);
    for (i = 0; i < nv; i++) {
        if (he_bnd_ver(he, i)) continue;
        status = he_ring(he, i, &n, &ring);
        if (status != HE_OK)
            ER("he_ring failed for i = %d", i);
        printf("%d %d\n", i, n);
        for (j = 0; j < n; j++) {
            fputs(j == 0 ? ": " : " ", stdout);
            printf("%d", ring[j]);
        }
        printf("\n");
    }
    return y_fin(he, x, y, z);
}
