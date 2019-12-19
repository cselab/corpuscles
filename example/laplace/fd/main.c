#include <stdio.h>

#include <real.h>
#include <co/err.h>
#include <co/he.h>
#include <co/laplace.h>
#include <co/memory.h>
#include <co/sum.h>
#include <co/vec.h>
#include <co/ten.h>
#include <co/y.h>

static He *he;
static Laplace *laplace;
static int n;
static real *x, *y, *z, *area;
static real *lx, *ly, *lz;
static real h = 1e-6;

enum {X, Y, Z};

static int Energy_sum(real s[3]) {
    laplace_apply(laplace, he, x, y, z, /**/ &lx, &ly, &lz, &area);
    s[X] = sum_array(n, lx);
    s[Y] = sum_array(n, ly);
    s[Z] = sum_array(n, lz);
    return CO_OK;
}

static int Energy_one(real s[3]) {
    laplace_apply(laplace, he, x, y, z, /**/ &lx, &ly, &lz, &area);
    s[X] = lx[0];
    s[Y] = ly[0];
    s[Z] = lz[0];
    return CO_OK;
}

static int Energy(real s[3]) { return Energy_one(s); }

static int fd0(real *p, /**/ real f[3]) {
    real t, hi[3], lo[3];
    t = *p;
    *p += h; Energy(hi); *p = t;
    *p -= h; Energy(lo); *p = t;

    f[X] = (hi[X] - lo[X])/(2*h);
    f[Y] = (hi[Y] - lo[Y])/(2*h);
    f[Z] = (hi[Z] - lo[Z])/(2*h);
    return CO_OK;
}

static int fd(int i, /**/ Ten *t) {
    real cx[3], cy[3], cz[3];
    fd0(&x[i], cx);
    fd0(&y[i], cy);
    fd0(&z[i], cz);
    ten_col_ini(cx, cy, cz, /**/ t);
    return CO_OK;
}

int main() {
    int i;
    real s[3], rr[3];
    Ten *dl;
    real *trace, *determinant, *r;

    y_inif(stdin, &he, &x, &y, &z);
    n = he_nv(he);
    MALLOC(n, &dl);
    MALLOC(n, &trace); MALLOC(n, &determinant);
    MALLOC(n, &r);
    laplace_ini(he, &laplace);

    for (i = 0; i < n; i++) {
        fd(i, &dl[i]);
        trace[i] = ten_trace(&dl[i]);
        determinant[i] = ten_determinant(&dl[i]);
        vec_get(i, x, y, z, /**/ rr);
        r[i] = vec_cylindrical_r(rr);
    }
    Energy(s);

    puts("x y z lx ly lz r trace determinant xx xy xz yx yy yz zx zy zz");
    for (i = 0; i < n; i++) {
        printf("%g %g %g ", x[i], y[i], z[i]);
        printf("%g %g %g ", lx[i], ly[i], lz[i]);
        printf("%g %g %g ", r[i], trace[i], determinant[i]);
        ten_line(&dl[i]);
        puts("");
    }

    laplace_fin(laplace);
    FREE(dl);
    FREE(trace); FREE(determinant);
    FREE(r);
    y_fin(he, x, y, z);
    return CO_OK;
}
