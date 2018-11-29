#include <stdio.h>
#include <stdlib.h>

#include <real.h>
#include <he/err.h>
#include <he/he.h>
#include <he/vec.h>
#include <he/normal.h>
#include <he/da.h>
#include <he/memory.h>
#include <he/punto.h>
#include <he/sum.h>
#include <he/ten.h>
#include <he/punto.h>
#include <he/macro.h>
#include <he/y.h>

static real dda(__UNUSED void *p, real area) { return  area; }

int main() {
    Da *da;
    He *he;
    int n, i;
    real *x, *y, *z, *rr, *area;
    real *fx, *fy, *fz, *ff;
    real r[3], f[3];
    dAParam param;
    param.da = dda;

    y_ini("/dev/stdin", &he, &x, &y, &z);
    n = he_nv(he);
    da_ini(he, &da);

    CALLOC(n, &fx); CALLOC(n, &fy); CALLOC(n, &fz); MALLOC(n, &ff);
    MALLOC(n, &rr);

    da_force(da, param, he, x, y, z, /**/ fx, fy, fz);
    da_area(da, &area);
    for (i = 0; i < n; i++) {
        vec_get(i, x, y, z, /**/ r);
        vec_get(i, fx, fy, fz, /**/ f);
        rr[i] = vec_cylindrical_r(r);
        ff[i] = vec_abs(f);
    }
    MSG("area: %g", he_sum_array(n, area));

    puts("x y z r fx fy fz ff area");
    real *queue[] = {x, y, z, rr, fx, fy, fz, ff, area, NULL};
    punto_fwrite(n, queue, stdout);

    da_fin(da);
    FREE(fx); FREE(fy); FREE(fz); FREE(ff); FREE(rr);
    y_fin(he, x, y, z);
    return HE_OK;
}
