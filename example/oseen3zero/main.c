#include <stdio.h>
#include <math.h>
#include <real.h>
#include <co/array.h>
#include <co/err.h>
#include <co/he.h>
#include <co/matrix.h>
#include <co/memory.h>
#include <co/oseen/3zero.h>
#include <co/y.h>

#define FMT CO_REAL_OUT

int
main(void)
{
    He *he;
    Oseen3Zero *oseen;
    real *x, *y, *z;
    real *S, *Oxx, *Oxy, *Oxz, *Oyy, *Oyz, *Ozz;
    real xx, yy, zz, s;
    int n, nt, i, j, k, t;

    err_set_ignore();
    y_inif(stdin, &he, &x, &y, &z);
    n = he_nv(he);
    nt = he_nt(he);

    matrix_ini(n, n, &S);
    matrix_ini(n, n, &Oxx);
    matrix_ini(n, n, &Oxy);
    matrix_ini(n, n, &Oxz);
    matrix_ini(n, n, &Oyy);
    matrix_ini(n, n, &Oyz);
    matrix_ini(n, n, &Ozz);
    oseen3_zero_ini(he, &oseen);
    oseen3_zero_apply(oseen, he, x, y, z, Oxx, Oxy, Oxz, Oyy, Oyz, Ozz);

    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++) {
            xx = matrix_get(n, n, i, j, Oxx);
            yy = matrix_get(n, n, i, j, Oyy);
            zz = matrix_get(n, n, i, j, Ozz);
            s = xx + yy + zz;
            matrix_set(n, n, i, j, s, S);
        }

    for (t = 0; t < nt; t++) {
        he_tri_ijk(he, t, &i, &j, &k);
        printf(FMT "\n", matrix_get(n, n, i, j, S));
        printf(FMT "\n", matrix_get(n, n, i, k, S));
    }
    matrix_fin(S);
    matrix_fin(Oxx);
    matrix_fin(Oxy);
    matrix_fin(Oxz);
    matrix_fin(Oyy);
    matrix_fin(Oyz);
    matrix_fin(Ozz);
    oseen3_zero_fin(oseen);
    y_fin(he, x, y, z);
}
