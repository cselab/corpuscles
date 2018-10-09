#include <stdio.h>
#include <math.h>

#include <real.h>

#include <he/f/juelicher.h>

#include <he/memory.h>
#include <he/punto.h>
#include <he/vec.h>
#include <he/err.h>
#include <he/x.h>

static real *FX, *FY, *FZ;
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
static real area_ver(real **p) { return f_juelicher_area_ver(p); }
static real curva_mean(real **p) { return f_juelicher_curva_mean(p); }

static void main0() {
    int i, nstep;
    real e0;
    real *eng, *area, *curv;

    e0 = energy(); force();
    e0 = energy(); force(); /* test */

    energy_ver(&eng);
    area_ver(&area);
    curva_mean(&curv);

    MSG("eng: %.5g", e0);
    real *queue[] = {XX, YY, ZZ, FX, FY, FZ, area, eng, curv, NULL};
    punto_fwrite(NV, queue, stdout);
}

int main() {
    real K, C0, Kad;
    K = 1; C0 = 0.0; Kad = 0;
    //Kad = 2*K/pi;

    ini("/dev/stdin");
    f_juelicher_ini(K, C0, Kad);

    RZERO(NV, &FX); RZERO(NV, &FY); RZERO(NV, &FZ);
    main0();
    FREE(FX); FREE(FY); FREE(FZ);
    f_juelicher_fin();
    fin();
    return 0;
}
