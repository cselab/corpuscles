#include <stdio.h>
#include <math.h>

#include <real.h>
#include <co/f/garea.h>
#include <co/memory.h>
#include <co/punto.h>
#include <co/x.h>

static real *FX, *FY, *FZ;

static void main0() {
    const real *queue[] = {XX, YY, ZZ, FX, FY, FZ, NULL};
    fprintf(stderr, "eng: %g\n", f_garea_energy(XX, YY, ZZ));
    f_garea_force(XX, YY, ZZ,   FX, FY, FZ);
    punto_fwrite(NV, queue, stdout);
}

int main() {
    real a0, K;
    a0 = 0.1; K = 1;
    ini("/dev/stdin");
    f_garea_ini(a0, K);

    RZERO(NV, &FX); RZERO(NV, &FY); RZERO(NV, &FZ);
    main0();

    FREE(FX); FREE(FY); FREE(FZ);
    f_garea_fin();
    fin();
    return 0;
}
