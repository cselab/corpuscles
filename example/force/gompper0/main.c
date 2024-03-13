#include <stdio.h>
#include <math.h>

#include <real.h>

#include <co/f/gompper.h>

#include <co/memory.h>
#include <co/punto.h>
#include <co/vec.h>
#include <co/err.h>
#include <co/macro.h>
#include <co/util.h>
#include <co/x.h>

#define FMT_IN   CO_REAL_IN

static real Kb, C0, Kad, DA0D;
static real *ENG, *AREA;
static real *FX, *FY, *FZ;
static const real Pi = 3.141592653589793115997964;
static const char **argv;
static const char *me = "force/gompper";

static void usg(void) {
    fprintf(stderr, "%s Kb C0, Kad DA0D < OFF\n", me);
    exit(0);
}
int eq(const char *a, const char *b) { return util_eq(a, b); }
int scl(/**/ real *p) {
    if (*argv == NULL) {
        MSG("not enough args");
        usg();
    }
    if (sscanf(*argv, FMT_IN, p) != 1)
        ER("not a number '%s'", *argv);
    argv++;
    return CO_OK;
}
static void arg() {
    if (*argv != NULL && eq(*argv, "-h")) usg();
    scl(&Kb); scl(&C0); scl(&Kad); scl(&DA0D);
}
static void zero(int n, real *a) {
    int i;
    for (i = 0; i < n; i++) a[i] = 0;
}
static void copy(int n, const real *a, real *b) {
    int i;
    for (i = 0; i < n; i++) b[i] = a[i];
}
static void vabs(int n, real *x, real *y, real *z, /**/ real *r) {
  /*Given n vectors with x, y, z components,
    calculate the absolute value/Euclidean length for each vector.*/
  
    int i;
    
    for (i = 0; i < n; i++)
        r[i] = sqrt(x[i]*x[i] + y[i]*y[i] + z[i]*z[i]);
    
}
static real energy() { return f_gompper_energy(XX, YY, ZZ); }
static void force() {
    zero(NV, FX); zero(NV, FY); zero(NV, FZ);
    f_gompper_force(XX, YY, ZZ,   FX, FY, FZ);
}

//static int energy_ver(real **pq) { return f_gompper_energy_ver(pq); }
//static int area_ver(real **pq) { return f_gompper_area(pq); }

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
    real *eng, *area;
    real *curva_mean;
    real *Fm;

    e0 = energy();
    
    f_gompper_energy_ver(&eng);
    f_gompper_area_ver(&area);
    f_gompper_curva_mean_ver(&curva_mean);

    RZERO(NV, &Fm);
    
    force();
    
    MSGR("FX[0]: %g", FX[0]);
    vabs(NV, FX, FY, FZ, /**/ Fm);
    MSGR("energy: %g", e0);
    MSGR("force : %g %g", FX[0], FX[NV-1]);

    //printf("#1 z; 2 axis dist; 3 eng; 4 Fx; 5 Fy; 6 Fz; 7 Fm; 8 area; 9 curva_mean \n");
    //real *queue[] = {ZZ, RR, eng, FX, FY, FZ, Fm, area, curva_mean, NULL};
    printf("x y z rxy eng Fx Fy Fz Fm area cm\n");
    const real *queue[] = {XX, YY, ZZ, RR, eng, FX, FY, FZ, Fm, area, curva_mean, NULL};
    punto_fwrite(NV, queue, stdout);

    FREE(Fm);
    
}

int main(int __UNUSED argc, const char *v[]) {
  argv = v; argv++;
  arg();
  ini("/dev/stdin");
  f_gompper_ini(Kb, C0, Kad, DA0D);
  
  RZERO(NV, &FX); RZERO(NV, &FY); RZERO(NV, &FZ);
  RZERO(NV, &ENG); RZERO(NV, &AREA);
  
  main0();
  
  FREE(FX); FREE(FY); FREE(FZ); FREE(ENG); FREE(AREA);
  f_gompper_fin();
  fin();
  return 0;
}
