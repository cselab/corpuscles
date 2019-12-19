#include <stdio.h>

#include <real.h>
#include <co/err.h>
#include <co/he.h>
#include <co/normal.h>
#include <co/memory.h>
#include <co/punto.h>
#include <co/sum.h>
#include <co/y.h>

int main() {
    He *he;
    int n, i;
    real *x, *y, *z;
    real *nx, *ny, *nz;
    real sx, sy, sz;

    y_inif(stdin, &he, &x, &y, &z);
    n = he_nv(he);
    MALLOC(n, &nx); MALLOC(n, &ny); MALLOC(n, &nz);

    for (i = 0; i < 100; i++)
	normal_mwa(he, x, y, z, /**/ nx, ny, nz);

    const real *queue[] = {x, y, z, nx, ny, nz, NULL};
    puts("x y z nx ny nz");
    punto_fwrite(n, queue, stdout);

    sx = sum_array(n, nx);
    sy = sum_array(n, ny);
    sz = sum_array(n, nz);

    MSG("n[0]: %g %g %g", nx[0], ny[0], nz[0]);
    MSG("n[n - 1]: %g %g %g", nx[n - 1], ny[n - 1], nz[n - 1]);
    MSG("sum:  %g %g %g", sx, sy, sz);

    FREE(nx); FREE(ny); FREE(nz);
    y_fin(he, x, y, z);
    return CO_OK;
}
