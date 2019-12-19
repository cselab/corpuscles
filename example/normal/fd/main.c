#include <stdio.h>

#include <real.h>
#include <co/err.h>
#include <co/he.h>
#include <co/normal.h>
#include <co/memory.h>
#include <co/sum.h>
#include <co/vec.h>
#include <co/ten.h>
#include <co/y.h>

static He *he;
static int n;
static real *x, *y, *z;
static real *nx, *ny, *nz;
static real h = 1e-8;

enum {X, Y, Z};

static int Energy(real s[3]) {
    normal_mwa(he, x, y, z, /**/ nx, ny, nz);
    s[X] = sum_array(n, nx);
    s[Y] = sum_array(n, ny);
    s[Z] = sum_array(n, nz);
    return CO_OK;
}

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
    Ten *dn;
    real *trace, *determinant, *r;

    y_inif(stdin, &he, &x, &y, &z);
    n = he_nv(he);
    MALLOC(n, &nx); MALLOC(n, &ny); MALLOC(n, &nz);
    MALLOC(n, &dn);
    MALLOC(n, &trace); MALLOC(n, &determinant);
    MALLOC(n, &r);


    for (i = 0; i < n; i++) {
        fd(i, &dn[i]);
        trace[i] = ten_trace(&dn[i]);
        determinant[i] = ten_determinant(&dn[i]);
        vec_get(i, x, y, z, /**/ rr);
        r[i] = vec_cylindrical_r(rr);
    }
    Energy(s);

    puts("x y z nx ny nz r trace determinant xx xy xz yx yy yz zx zy zz");
    for (i = 0; i < n; i++) {
        printf("%g %g %g ", x[i], y[i], z[i]);
        printf("%g %g %g ", nx[i], ny[i], nz[i]);
        printf("%g %g %g ", r[i], trace[i], determinant[i]);
        ten_line(&dn[i]);
        puts("");
    }

    FREE(nx); FREE(ny); FREE(nz); FREE(dn);
    FREE(trace); FREE(determinant);
    FREE(r);
    y_fin(he, x, y, z);
    return CO_OK;
}
