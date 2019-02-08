#include <stdio.h>

#include <real.h>
#include <he/err.h>
#include <he/he.h>
#include <he/laplace.h>
#include <he/memory.h>
#include <he/punto.h>
#include <he/sum.h>
#include <he/y.h>

int main() {
    Laplace *laplace;
    He *he;
    int n;
    real *x, *y, *z;
    real *lx, *ly, *lz, *area;
    real sx, sy, sz, A;

    y_inif(stdin, &he, &x, &y, &z);
    n = he_nv(he);
    laplace_ini(he, &laplace);
    laplace_apply(laplace, he, x, y, z, &lx, &ly, &lz, &area);

    const real *queue[] = {x, y, z, lx, ly, lz, area, NULL};
    puts("x y z lx ly lz area");
    punto_fwrite(n, queue, stdout);

    sx = he_sum_array(n, lx);
    sy = he_sum_array(n, ly);
    sz = he_sum_array(n, lz);
    A  = he_sum_array(n, area);

    MSG("l[0]: %g %g %g", lx[0], ly[0], lz[0]);
    MSG("l[n - 1]: %g %g %g", lx[n - 1], ly[n - 1], lz[n - 1]);
    MSG("sum:  %g %g %g", sx, sy, sz);
    MSG("area: %g", A);

    laplace_fin(laplace);
    y_fin(he, x, y, z);
    return HE_OK;
}
