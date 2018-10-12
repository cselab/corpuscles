#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include <real.h>

#include <he/f/meyer.h>

#include <he/memory.h>
#include <he/punto.h>
#include <he/vec.h>
#include <he/err.h>
#include <he/macro.h>
#include <he/util.h>
#include <he/x.h>

#define FMT_IN   XE_REAL_IN

static real Kb, C0, Kad, DA0D;
static real *ENG;
static real *FX, *FY, *FZ;
static const real Pi = 3.141592653589793115997964;
static const char **argv;
static const char *me = "force/meyer";

static void usg() {
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
    return HE_OK;
}
static void arg() {
    if (*argv != NULL && eq(*argv, "-h")) usg();
    scl(&Kb); scl(&C0); scl(&Kad); scl(&DA0D);
}
static void zero(int n, real *a) {
    int i;
    for (i = 0; i < n; i++) a[i] = 0;
}
static void vabs(int n, real *x, real *y, real *z, /**/ real *r) {
  /*Given n vectors with x, y, z components,
    calculate the absolute value/Euclidean length for each vector.*/
  
    int i;
    
    for (i = 0; i < n; i++)
        r[i] = sqrt(x[i]*x[i] + y[i]*y[i] + z[i]*z[i]);
    
}
static real energy() { return f_meyer_energy(XX, YY, ZZ); }
//static real energy_ver(real **p) { return f_meyer_energy_ver(p); }
//static real fad(real **x, real **y, real **z) { return f_meyer_fad(x, y, z); }
static int force() {
    zero(NV, FX); zero(NV, FY); zero(NV, FZ);
    return f_meyer_force(XX, YY, ZZ, FX, FY, FZ);
}
static void main0() {
    int i, nstep;
    real e0;
    real *eng, *area;
    real *curva_mean;
    real *curva_gauss;
    real *fm, *fmad;

    e0 = energy();
    
    f_meyer_energy_ver(&eng);
    f_meyer_area_ver(&area);
    f_meyer_curva_mean_ver(&curva_mean);
    f_meyer_curva_gauss_ver(&curva_gauss);

    RZERO(NV, &fm);
    RZERO(NV, &fmad);
    force();
    MSG("FX[0]: %g", FX[0]);
    vabs(NV, FX, FY, FZ, /**/ fm);
    MSG("energy: %g", e0);
    MSG("force : %g %g", FX[0], FX[NV-1]);

    printf("#1 z; 2 axis dist; 3 eng; 4 Fx; 5 Fy; 6 Fz; 7 fm; 8 area; 9 curva_mean; 10 curva_gauss \n");
    real *queue[] = {ZZ, RR, eng, FX, FY, FZ, fm, area, curva_mean, curva_gauss, NULL};
    punto_fwrite(NV, queue, stdout);

    FREE(fm);
    FREE(fmad);
}
int main(int __UNUSED argc, const char *v[]) {
  argv = v; argv++;
  arg();
  ini("/dev/stdin");
  f_meyer_ini(Kb, C0, Kad, DA0D);
  
  RZERO(NV, &FX); RZERO(NV, &FY); RZERO(NV, &FZ);
  
  main0();
  
  FREE(FX); FREE(FY); FREE(FZ);
  
  f_meyer_fin();
  fin();
  return 0;
}
