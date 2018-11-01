#include <stdio.h>
#include <math.h>

#include <real.h>
#include <he/f/kantor.h>
#include <he/memory.h>
#include <he/punto.h>
#include <he/x.h>

static real *FX, *FY, *FZ;

static void main0() {
    real *queue[] = {XX, YY, ZZ, FX, FY, FZ, NULL};
    fprintf(stderr, "eng: %g\n", f_kantor_energy(XX, YY, ZZ));
    f_kantor_force(XX, YY, ZZ,   FX, FY, FZ);
    puts("x y z fx fy fz");
    punto_fwrite(NV, queue, stdout);
}

int main() {
    real K, theta0;
    K = 1; theta0 = 0;
    ini("/dev/stdin");
    f_kantor_ini(K, theta0);

    RZERO(NV, &FX); RZERO(NV, &FY); RZERO(NV, &FZ);
    main0();

    FREE(FX); FREE(FY); FREE(FZ);
    f_kantor_fin();
    fin();
    return 0;
}
