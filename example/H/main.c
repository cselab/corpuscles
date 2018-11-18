#include <stdio.h>

#include <real.h>
#include <he/err.h>
#include <he/he.h>
#include <he/H.h>
#include <he/memory.h>
#include <he/punto.h>
#include <he/sum.h>
#include <he/vec.h>
#include <he/y.h>

int main() {
    He *he;
    int n, i;
    real *x, *y, *z;
    real *hh, *area, *rr, s;
    H *h;
    real r[3];

    y_ini("/dev/stdin", &he, &x, &y, &z);
    n = he_nv(he);
    MALLOC(n, &rr);
    H_ini(he, /**/ &h);

    H_apply(h, he, x, y, z, /**/ &hh, &area);

    for (i = 0; i < n; i++) {
        vec_get(i, x, y, z, /**/ r);        
        rr[i] = vec_cylindrical_r(r);
    }

    real *queue[] = {x, y, z, rr, hh, area, NULL};
    puts("x y z r H area");
    punto_fwrite(n, queue, stdout);
    s = he_sum_array(n, hh);

    MSG("h[0]: %g", hh[0]);
    MSG("h[n - 1]: %g", hh[n - 1]);
    MSG("sum: %g", s);

    FREE(rr);
    H_fin(h);
    y_fin(he, x, y, z);
    return HE_OK;
}
