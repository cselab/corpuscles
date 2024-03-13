#include <stdio.h>
#include <math.h>

#include <real.h>

#include <co/err.h>
#include <co/vec.h>
#include <co/punto.h>

/* #include <co/x.h> */
#include <alg/min.h>
#include <alg/x.h>

static real R0 = 1.0;

static real sq(real x) { return x*x; }
real Energy(const real *xx, const real *yy, const real *zz) {
    real e, r[3];
    int i;
    e = 0;
    for (i = 0; i < NV; i++) {
        vec_get(i, xx, yy, zz, r);
        e += sq(vec_dot(r, r) - R0);
    }
    return e;
}

void Force(const real *xx, const real *yy, const real *zz, /**/
           real *fx, real *fy, real *fz) {
    real r[3], r2, f[3];
    int i;
    for (i = 0; i < NV; i++) {
        vec_get(i, xx, yy, zz, r);
        r2 = vec_dot(r, r);
        vec_scalar(r, 4*(r2 - R0), f);
        vec_set(f, i, /**/ fx, fy, fz);
    }
}

static void main0() {
    const real *queue[] = {XX, YY, ZZ, NULL};
    do {
        min_iterate();
        fprintf(stderr, "%.16e\n", min_energy());
        min_position(/**/ XX, YY, ZZ);
        punto_fwrite(NV, queue, stdout);
        printf("\n");
    } while (!min_end());

}

int main() {
    ini("/dev/stdin");
    min_ini(STEEPEST_DESCENT);

    main0();

    min_fin();
    fin();
    return 0;
}
