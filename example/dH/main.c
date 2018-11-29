#include <stdio.h>

#include <real.h>
#include <he/err.h>
#include <he/he.h>
#include <he/vec.h>
#include <he/normal.h>
#include <he/dH.h>
#include <he/memory.h>
#include <he/punto.h>
#include <he/sum.h>
#include <he/ten.h>
#include <he/punto.h>
#include <he/y.h>

static real ddh(void *p, real area, real H) { return   4*H/area; }
static real dda(void *p, real area, real H) { return  -(2*H*H)/(area*area); }

int main() {
    Dh *dh;
    He *he;
    int n, i;
    real *x, *y, *z, *rr, *area, *H;
    real *fx, *fy, *fz, *ff;
    real r[3], f[3];
    dHParam param;
    param.dh = ddh;
    param.da = dda;

    y_ini("/dev/stdin", &he, &x, &y, &z);
    n = he_nv(he);
    dh_ini(he, &dh);
    CALLOC(n, &fx); CALLOC(n, &fy); CALLOC(n, &fz); MALLOC(n, &ff);
    MALLOC(n, &rr);

    dh_force(dh, param, he, x, y, z, /**/ fx, fy, fz);
    dh_area(dh, &area);
    dh_h(dh, &H);

    for (i = 0; i < n; i++) {
        vec_get(i, x, y, z, /**/ r);
        vec_get(i, fx, fy, fz, /**/ f);
        rr[i] = vec_cylindrical_r(r);
        ff[i] = vec_abs(f);
    }

    MSG("area: %g", he_sum_array(n, area));

    puts("x y z r fx fy fz ff area H");
    real *queue[] = {x, y, z, rr, fx, fy, fz, ff, area, H, NULL};
    punto_fwrite(n, queue, stdout);

    dh_fin(dh);
    FREE(fx); FREE(fy); FREE(fz); FREE(ff); FREE(rr);
    y_fin(he, x, y, z);
    return HE_OK;
}