#include <stdio.h>
#include <math.h>

#include <real.h>
#include <he/f/gompper.h>
#include <he/memory.h>
#include <he/punto.h>
#include <he/vec.h>
#include <he/x.h>

static real *FX, *FY, *FZ;

static void zero(int n, real *a) {
    int i;
    for (i = 0; i < n; i++) a[i] = 0;
}

static void force() {
    zero(NV, FX); zero(NV, FY); zero(NV, FZ);
    f_gompper_force(XX, YY, ZZ,   FX, FY, FZ);
}

static void update() {
    real d, f[3];
    int i;

    d = 5e-5;
    for (i = 0; i < NV; i++) {
        XX[i] += d*FX[i];
        YY[i] += d*FY[i];
        ZZ[i] += d*FZ[i];
    }
    vec_get(0, FX, FY, FZ, f);
    vec_fprintf(f, stderr, "%g");
}

static void main0() {
    int i, nstep;
    real e0;
    real *queue[] = {XX, YY, ZZ, FX, FY, FZ, NULL};

    nstep = 100;
    e0 = f_gompper_energy(XX, YY, ZZ);

    for (i = 0; i < nstep; i++) {
        force();
        update();
        fprintf(stderr, "eng: %.5f\n",
                (f_gompper_energy(XX, YY, ZZ) - e0)/e0);
    }
    
    punto_fwrite(NV, queue, stdout);
    putchar('\n');
}

int main() {
    real K;
    K = 1;
    ini("/dev/stdin");
    f_gompper_ini(K);

    RZERO(NV, &FX); RZERO(NV, &FY); RZERO(NV, &FZ);
    main0();

    FREE(FX); FREE(FY); FREE(FZ);
    f_gompper_fin();
    fin();
    return 0;
}
