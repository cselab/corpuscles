#include <stdio.h>
#include <math.h>

#include <real.h>
#include <he/f/area.h>
#include <he/memory.h>
#include <he/x.h>

static real *FX, *FY, *FZ;

void main0() {
    printf("%g\n", f_area_energy(XX, YY, ZZ));
    f_area_force(XX, YY, ZZ,   FX, FY, FZ);
    printf("[%g %g %g]\n", FX[NV - 2], FY[NV - 2], FZ[NV - 2]);
}

int main() {
    real a0, K;
    a0 = 100; K = 1;
    ini("/dev/stdin");
    f_area_ini(a0, K);

    RZERO(NV, &FX); RZERO(NV, &FY); RZERO(NV, &FZ);
    main0();

    FREE(FX); FREE(FY); FREE(FZ);
    f_area_fin();
    fin();
    return 0;
}
