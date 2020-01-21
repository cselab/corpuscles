#include <stdio.h>
#ifdef _OPENMP
#include <omp.h>
#endif
#include "real.h"
#include "co/err.h"
#include "co/matrix.h"
#include "co/ten.h"
#include "co/i/matrix.h"
#include "co/tensor3x3.h"

enum { XX, XY, XZ, YX, YY, YZ, ZX, ZY, ZZ };

int
tensor3x3_ini(int n, struct Tensor3x3 *t)
{
    int i;
    int status;

    for (i = 0; i < 3 * 3; i++) {
        status = matrix_ini(n, n, &t->d[i]);
        if (status != CO_OK)
            ERR(CO_MEMORY, "matrix_ini failed (n = %d)", n);
    }
    return CO_OK;
}

int
tensor3x3_zero(int n, struct Tensor3x3 *t)
{
    int i;
    int status;

    for (i = 0; i < 3 * 3; i++)
        matrix_zero(n, n, t->d[i]);
    return CO_OK;
}

int
tensor3x3_scale(int n, real s, struct Tensor3x3 *t)
{
    int i;
    int status;

    for (i = 0; i < 3 * 3; i++)
        matrix_scale(n, n, s, t->d[i]);
    return CO_OK;
}

int
tensor3x3_fin(struct Tensor3x3 *t)
{
    int i;

    for (i = 0; i < 3 * 3; i++)
        matrix_fin(t->d[i]);
    return CO_OK;
}

int
tensor3x3_vector(int n, real s, const real * x, const real * y,
                 const real * z, struct Tensor3x3 *T, real * u, real * v,
                 real * w)
{
    int i;

#define GET(K) i_matrix_get(n, n, i, j, (T->d[K]))
#pragma omp parallel for
    for (i = 0; i < n; i++) {
        int j;
        real xx, xy, xz, yx, yy, yz, zx, zy, zz, du, dv, dw;
        real a, b, c;

        a = b = c = 0;
        for (j = 0; j < n; j++) {
            xx = GET(XX);
            xy = GET(XY);
            xz = GET(XZ);
            yx = GET(YX);
            yy = GET(YY);
            yz = GET(YZ);
            zx = GET(ZX);
            zy = GET(ZY);
            zz = GET(ZZ);
            du = xx * x[j] + xy * y[j] + xz * z[j];
            dv = yx * x[j] + yy * y[j] + yz * z[j];
            dw = zx * x[j] + zy * y[j] + zz * z[j];
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
