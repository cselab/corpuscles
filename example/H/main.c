#include <stdio.h>

#include <real.h>
#include <co/err.h>
#include <co/he.h>
#include <co/H.h>
#include <co/memory.h>
#include <co/punto.h>
#include <co/sum.h>
#include <co/vec.h>
#include <co/y.h>

enum {X, Y, Z};

static const real step = 1e-6;
static He *he;
static int n;
static H *h;
static real *x, *y, *z;

static real Energy_sum() {
    real *hh, *area;
    H_apply(h, he, x, y, z, /**/ &hh, &area);
    return sum_array(n, hh);
}

static real Energy_one() {
    real *hh, *area;
    H_apply(h, he, x, y, z, /**/ &hh, &area);
    return hh[0];
}

static real Energy() { return Energy_sum(); }

static real fd0(real *p) {
    real t, hi, lo;
    t = *p;
    *p += step; hi = Energy(); *p = t;
    *p -= step; lo = Energy(); *p = t;
    return (hi - lo)/(2*step);
}

static int fd(int i, /**/ real v[3]) {
    v[X] = fd0(&x[i]);
    v[Y] = fd0(&y[i]);
    v[Z] = fd0(&z[i]);
    return CO_OK;
}

int main() {
    int i;
    real *rr, *dh, s;
    real r[3], f[3], *fx, *fy, *fz, *ff;
    real *hh, *area;

    y_inif(stdin, &he, &x, &y, &z);
    n = he_nv(he);

    MALLOC(n, &rr);
    MALLOC(n, &fx); MALLOC(n, &fy); MALLOC(n, &fz); MALLOC(n, &ff);

    H_ini(he, /**/ &h);

    H_apply(h, he, x, y, z, /**/ &hh, &area);

    for (i = 0; i < n; i++) {
        fd(i, f);
        vec_get(i, x, y, z, /**/ r);
        rr[i] = vec_cylindrical_r(r);
        vec_set(f, i, /**/ fx, fy, fz);
        ff[i] = vec_abs(f);
    }

    const real *queue[] = {x, y, z, rr, hh, area, fx, fy, fz, ff, NULL};
    puts("x y z r H area fx fy fz ff");
    punto_fwrite(n, queue, stdout);
    s = sum_array(n, hh);

    MSG("%g", ff[0]/area[0]);
    FREE(rr);
    FREE(fx); FREE(fy); FREE(fz); FREE(ff);

    H_fin(h);
    y_fin(he, x, y, z);
    return CO_OK;
}
