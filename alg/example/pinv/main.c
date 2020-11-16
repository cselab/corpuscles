#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <real.h>
#include <co/err.h>
#include <co/macro.h>

#include <alg/pinv.h>

#define OUT CO_REAL_OUT
#define IN  CO_REAL_IN

int
main(__UNUSED int argc, const char **argv)
{
    AlgPinv *pinv;
    real A[99], B[999];
    int dim, i, j, m;

    if (scanf("%d", &dim) != 1)
        ER("fail to read dimension");
    for (i = 0; i < dim * dim; i++)
        if (scanf(IN, &A[i]) != 1)
            ER("fail to read matrix");
    alg_pinv_ini(dim, &pinv);
    alg_pinv_apply(pinv, A, /**/ B);

    printf("%d\n", dim);
    for (i = m = 0; i < dim; i++) {
        for (j = 0; j < dim; j++) {
            if (j > 0)
                printf(" ");
            printf(OUT, B[m++]);
        }
        printf("\n");
    }
    alg_pinv_fin(pinv);
}
