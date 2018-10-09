#include <stdio.h>
#include <math.h>

#include <real.h>

#include <he/f/juelicher.h>

#include <he/memory.h>
#include <he/punto.h>
#include <he/vec.h>
#include <he/err.h>
#include <he/x.h>

static real *FX, *FY, *FZ, *ENG, *AREA;
static const real Pi = 3.141592653589793115997964;

static void zero(int n, real *a) {
    int i;
    for (i = 0; i < n; i++) a[i] = 0;
}
static void force() {
    zero(NV, FX); zero(NV, FY); zero(NV, FZ);
    f_juelicher_force(XX, YY, ZZ,   FX, FY, FZ);
}

static real energy() { return f_juelicher_energy(XX, YY, ZZ); }
static real energy_ver(real **p) { return f_juelicher_energy_ver(p); }

static void main0() {
    int i, nstep;
    real e0;
    real *eng;

    e0 = energy();
    force();
    f_juelicher_energy_ver(&eng);

    MSG("eng: %.5g", e0);
    real *queue[] = {XX, YY, ZZ, FX, FY, FZ, eng, NULL};
    punto_fwrite(NV, queue, stdout);
}

int main() {
    real K, C0, Kad;
    K = 1;
    C0 = -1.0;
    Kad = 2*K/pi;
    ini("/dev/stdin");
    f_juelicher_ini(K, C0, Kad);

    RZERO(NV, &FX); RZERO(NV, &FY); RZERO(NV, &FZ); RZERO(NV, &ENG); RZERO(NV, &AREA);
    main0();
    FREE(FX); FREE(FY); FREE(FZ); FREE(ENG); FREE(AREA);
    f_juelicher_fin();
    fin();
    return 0;
}
