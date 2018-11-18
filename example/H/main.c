#include <stdio.h>

#include <real.h>
#include <he/err.h>
#include <he/he.h>
#include <he/H.h>
#include <he/memory.h>
#include <he/punto.h>
#include <he/sum.h>
#include <he/y.h>

int main() {
    He *he;
    int n;
    real *x, *y, *z;
    real *hh, *area, s;
    H *h;

    y_ini("/dev/stdin", &he, &x, &y, &z);
    n = he_nv(he);
    H_ini(he, /**/ &h);

    H_apply(h, he, x, y, z, /**/ &hh, &area);

    real *queue[] = {x, y, z, hh, NULL};
    puts("x y z H");
    punto_fwrite(n, queue, stdout);
    s = he_sum_array(n, hh);

    MSG("h[0]: %g", hh[0]);
    MSG("h[n - 1]: %g", hh[n - 1]);
    MSG("sum: %g", s);

    H_fin(h);

    y_fin(he, x, y, z);
    return HE_OK;
}
