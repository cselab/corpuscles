#include <stdio.h>

#include <real.h>
#include <he/err.h>
#include <he/he.h>
#include <he/normal.h>
#include <he/memory.h>
#include <he/punto.h>
#include <he/sum.h>
#include <he/ten.h>
#include <he/y.h>

int main() {
    He *he;
    int n;
    real *x, *y, *z;
    real *nx, *ny, *nz;
    real sx, sy, sz;
    Ten *dn;

    y_ini("/dev/stdin", &he, &x, &y, &z);
    n = he_nv(he);
    MALLOC(n, &nx); MALLOC(n, &ny); MALLOC(n, &nz);
    MALLOC(n, &dn);

    he_normal_mwa(he, x, y, z, /**/ nx, ny, nz);

    real *queue[] = {x, y, z, nx, ny, nz, NULL};
    puts("x y z nx ny nz");
    punto_fwrite(n, queue, stdout);

    sx = he_sum_array(n, nx);
    sy = he_sum_array(n, ny);
    sz = he_sum_array(n, nz);

    MSG("n[0]: %g %g %g", nx[0], ny[0], nz[0]);
    MSG("n[n - 1]: %g %g %g", nx[n - 1], ny[n - 1], nz[n - 1]);
    MSG("sum:  %g %g %g", sx, sy, sz);

    FREE(nx); FREE(ny); FREE(nz); FREE(dn);
    y_fin(he, x, y, z);
    return HE_OK;
}
