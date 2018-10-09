#include <stdio.h>
#include <math.h>

#include <real.h>

#include <he/f/gompper.h>

#include <he/memory.h>
#include <he/punto.h>
#include <he/vec.h>
#include <he/x.h>

static real *FX, *FY, *FZ, *ENG, *AREA;

static void zero(int n, real *a) {
    int i;
    for (i = 0; i < n; i++) a[i] = 0;
}
static void copy(int n, const real *a, real *b) {
    int i;
    for (i = 0; i < n; i++) b[i] = a[i];
}

static void force() {
    zero(NV, FX); zero(NV, FY); zero(NV, FZ);
    f_gompper_force(XX, YY, ZZ,   FX, FY, FZ);
}

static real energy() { return f_gompper_energy(XX, YY, ZZ); }

static int energy_ver(real **pq) { return f_gompper_energy_ver(pq); }
static int area_ver(real **pq) { return f_gompper_area(pq); }

static void update() {
    real d;
    int i;

    d = 1e-3;
    for (i = 0; i < NV; i++) {
        XX[i] -= d*FX[i];
        YY[i] -= d*FY[i];
        ZZ[i] -= d*FZ[i];
    }
}

static void main0() {
    int i, nstep;
    real e0;
    real *queue[] = {XX, YY, ZZ, FX, FY, FZ, ENG, AREA, NULL};
    real *eng, *area0;

    nstep = 100;
    e0 = energy();
    for (i = 0; i < nstep; i++) {
        force();
        update();
        fprintf(stderr, "eng: %.5f\n", (energy() - e0)/e0);
        energy_ver(&eng);  copy(NV, eng, ENG);
        area_ver(&area0); copy(NV, area0, AREA);

        punto_fwrite(NV, queue, stdout);
        putchar('\n');
    }
}

int main() {
    real K;
    K = 1;
    ini("/dev/stdin");
    f_gompper_ini(K);

    RZERO(NV, &FX); RZERO(NV, &FY); RZERO(NV, &FZ); RZERO(NV, &ENG); RZERO(NV, &AREA);
    main0();

    FREE(FX); FREE(FY); FREE(FZ); FREE(ENG); FREE(AREA);
    f_gompper_fin();
    fin();
    return 0;
}
