#include <stdio.h>

#include <real.h>
#include <co/err.h>
#include <co/he.h>
#include <co/ring.h>
#include <co/y.h>

#define FMT CO_REAL_OUT

static int write(int n, real *A) {
    int j, s;
    for (j = s = 0; s < n; s++) {
        printf(FMT " ", A[j++]);
        printf(FMT " ", A[j++]);
        printf(FMT " ", A[j++]);
        printf(FMT " ", A[j++]);
        printf(FMT " ", A[j++]);
        printf(FMT "\n",A[j++]);
    }
}

int main() {
    real *x, *y, *z;
    He      *he;
    int nv, nt, i, j, n, s, *rring, status;
    Ring *ring;
    real *alpha, *beta, *theta, *xyz, *A, *B;

    err_set_ignore();

    y_inif(stdin, &he, &x, &y, &z);
    ring_ini(&ring);

    nv = he_nv(he);

    for (i = 0; i < nv; i++) {
        if (he_bnd_ver(he, i)) continue;
        status = he_ring(he, i, &n, &rring);
        if (status != CO_OK)
            ER("he_ring failed for i = %d", i);

        ring_alpha(ring, i, rring, x, y, z, &alpha);
        ring_beta(ring, i, rring, x, y, z, &beta);
        ring_theta(ring, i, rring, x, y, z, &theta);
        ring_xyz(ring, i, rring, x, y, z, &xyz);
        ring_A(ring, i, rring, x, y, z, &A);
        ring_B(ring, i, rring, x, y, z, &B);

        for (s = 0; s < n; s++)
            printf("ang: " FMT " " FMT " " FMT "\n",
                   alpha[s], beta[s], theta[s]);

        for (j = s = 0; s < n + 1; s++) {
            printf("xyz: ");
            printf(FMT " ", xyz[j++]);
            printf(FMT " ", xyz[j++]);
            printf(FMT "\n", xyz[j++]);
        }
        printf("A:\n");
        write(n + 1, A);
        printf("B:\n");        
        write(6, B);
    }

    ring_fin(ring);
    return y_fin(he, x, y, z);
}
