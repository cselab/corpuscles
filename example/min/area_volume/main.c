#include <stdio.h>
#include <math.h>

#include <real.h>

#include <he/err.h>
#include <he/vec.h>
#include <he/punto.h>

#include <he/x.h>
#include <alg/x.h>
#include <alg/min.h>

real Energy(const real *x, const real *y, const real *z) {
    real a, v;
    MSG("x[0]: %g", x[0]);
    v = f_volume_energy(x, y, z);
    a = f_area_energy(x, y, z);
    MSG("av: %g %g", a, v);
    return a + v;
}

static void zero(int n, real *a) {
    int i;
    for (i = 0; i < n; i++) a[i] = 0;
}
void Force(const real *x, const real *y, const real *z, /**/
           real *fx, real *fy, real *fz) {
    zero(NV, fx); zero(NV, fy); zero(NV, fz);
    f_area_force(x, y, z, /**/ fx, fy, fz);
    f_volume_force(x, y, z, /**/ fx, fy, fz);
}

static void main0() {
    int i;
//    real *queue[] = {XX, YY, ZZ, NULL};
//    punto_fwrite(NV, queue, stdout);
//    printf("\n");
    for (i = 0; i < 10; i++) {
        min_position(/**/ XX, YY, ZZ);
//        punto_fwrite(NV, queue, stdout);
//        printf("\n");
        min_iterate();
        MSG("%.16e", min_energy());
    }

}

int main() {
    real v0, Kv, a0, Ka;
    ini("/dev/stdin");

    a0 = 0.006809515625;  Ka = 1;
    v0 = 1.5606;          Kv = 1;

    f_volume_ini(v0, Kv);
    f_area_ini(a0, Ka);
    min_ini(STEEPEST_DESCENT);

    main0();

    min_fin();
    f_volume_fin();
    f_area_fin();
    fin();
    return 0;
}
