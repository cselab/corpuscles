#include <stdio.h>
#ifdef _OPENMP
#include <omp.h>
#endif
#include "real.h"
#include "co/err.h"
#include "co/matrix.h"
#include "co/i/matrix.h"
#include "co/tensor.h"

int
tensor_ini(int n, struct Tensor *t)
{
    matrix_ini(n, n, &t->xx);
    matrix_ini(n, n, &t->xy);
    matrix_ini(n, n, &t->xz);
    matrix_ini(n, n, &t->yy);
    matrix_ini(n, n, &t->yz);
    matrix_ini(n, n, &t->zz);
    return CO_OK;
}

int
tensor_fin(struct Tensor *t)
{
    matrix_fin(t->xx);
    matrix_fin(t->xy);
    matrix_fin(t->xz);
    matrix_fin(t->yy);
    matrix_fin(t->yz);
    matrix_fin(t->zz);
    return CO_OK;
}

int
tensor_vector(int n, real s, const real * x, const real * y,
              const real * z, struct Tensor *T, real * u, real * v,
              real * w)
{
    int i;

#define GET(K) i_matrix_get(n, n, i, j, (T->K))
#pragma omp parallel for
    for (i = 0; i < n; i++) {
        int j;
        real xx, xy, xz, yy, yz, zz, du, dv, dw;
        real a, b, c;

        a = b = c = 0;
        for (j = 0; j < n; j++) {
            xx = GET(xx);
            xy = GET(xy);
            xz = GET(xz);
            yy = GET(yy);
            yz = GET(yz);
            zz = GET(zz);
            du = xx * x[j] + xy * y[j] + xz * z[j];
            dv = xy * x[j] + yy * y[j] + yz * z[j];
            dw = xz * x[j] + yz * y[j] + zz * z[j];
            a += du;
            b += dv;
            c += dw;
        }
        u[i] += s * a;
        v[i] += s * b;
        w[i] += s * c;
    }
    return CO_OK;
#undef GET
}
