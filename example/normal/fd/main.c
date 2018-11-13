#include <stdio.h>

#include <real.h>
#include <he/err.h>
#include <he/he.h>
#include <he/normal.h>
#include <he/memory.h>
#include <he/punto.h>
#include <he/sum.h>
#include <he/ten.h>
#include <he/vec.h>
#include <he/y.h>

static He *he;
static int n;
static real *x, *y, *z;
static real *nx, *ny, *nz;
static real h = 1e-6;

enum {X, Y, Z};

static int Energy(real s[3]) {
    he_normal_mwa(he, x, y, z, /**/ nx, ny, nz);
    s[X] = he_sum_array(n, nx);
    s[Y] = he_sum_array(n, ny);
    s[Z] = he_sum_array(n, nz);
    return HE_OK;
}

static int fd0(real *p, /**/ real f[3]) {
    real t, hi[3], lo[3];
    t = *p;
    *p += h; Energy(hi); *p = t;
    *p -= h; Energy(lo); *p = t;

    f[X] = (hi[X] - lo[X])/(2*h);
    f[Y] = (hi[Y] - lo[Y])/(2*h);
    f[Z] = (hi[Z] - lo[Z])/(2*h);
    return HE_OK;
}

static int fd(int i, /**/ Ten *t) {
    real cx[3], cy[3], cz[3];
    fd0(&x[i], cx);
    fd0(&y[i], cy);
    fd0(&z[i], cz);
    return HE_OK;
}

int main() {
    int i;
    real s[3];
    Ten *dn;

    y_ini("/dev/stdin", &he, &x, &y, &z);
    n = he_nv(he);
    MALLOC(n, &nx); MALLOC(n, &ny); MALLOC(n, &nz);
    MALLOC(n, &dn);

    real *queue[] = {x, y, z, nx, ny, nz, NULL};
    puts("x y z nx ny nz");
    //punto_fwrite(n, queue, stdout);
    Energy(s);

    for (i = 0; i < n; i++)
        fd(i, &dn[i]);

    FREE(nx); FREE(ny); FREE(nz); FREE(dn);
    y_fin(he, x, y, z);
    return HE_OK;
}
