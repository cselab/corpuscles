#include <stdio.h>

#include <real.h>
#include <he/err.h>
#include <he/he.h>
#include <he/normal.h>
#include <he/memory.h>
#include <he/punto.h>
#include <he/y.h>

int main() {
    He *he;
    int n;
    real *x, *y, *z;
    real *nx, *ny, *nz;
    real A;
    y_ini("/dev/stdin", &he, &x, &y, &z);
    n = he_nv(he);
    MALLOC(n, &nx); MALLOC(n, &ny); MALLOC(n, &nz);
    he_normal_mwa(he, x, y, z, /**/ nx, ny, nz);

    real *queue[] = {x, y, z, nx, ny, nz, NULL};
    puts("x y z nx ny nz");
    punto_fwrite(n, queue, stdout);

    FREE(nx); FREE(ny); FREE(nz);
    y_fin(he, x, y, z);
    return HE_OK;
}
