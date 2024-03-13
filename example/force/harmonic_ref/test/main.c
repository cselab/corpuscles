#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include <real.h>
#include <co/memory.h>
#include <co/punto.h>
#include <co/x.h>

static real *FX, *FY, *FZ;

static void main0() {
    const real *queue[] = {XX, YY, ZZ, FX, FY, FZ, NULL};
    fprintf(stderr, "eng: %g\n", f_harmonic_ref_energy(XX, YY, ZZ));
    f_harmonic_ref_force(XX, YY, ZZ,   FX, FY, FZ);
    punto_fwrite(NV, queue, stdout);
}

int main() {
    real a0, K;
    a0 = 0.1; K = 1;
    ini("/dev/stdin");
    f_harmonic_ref_ini(K, XX, YY, ZZ);

    RZERO(NV, &FX); RZERO(NV, &FY); RZERO(NV, &FZ);
    main0();

    FREE(FX); FREE(FY); FREE(FZ);
    f_harmonic_ref_fin();
    fin();
    return 0;
}
