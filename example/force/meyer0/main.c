#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include <real.h>

#include <co/f/meyer.h>

#include <co/memory.h>
#include <co/punto.h>
#include <co/vec.h>
#include <co/err.h>
#include <co/macro.h>
#include <co/util.h>
#include <co/x.h>

#define FMT_IN   CO_REAL_IN

static real Kb, C0, Kad, DA0D;
static real *ENG;
static real *FX, *FY, *FZ;
static const real Pi = 3.141592653589793115997964;
static const char **argv;
static const char *me = "force/meyer";

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
    real *Fm;
    real *lbx, *lby, *lbz, *lbm;
    real *nx, *ny, *nz, *nm;
    
  
    e0 = energy();
    force();
    
    RZERO(NV, &Fm);
    RZERO(NV, &lbm);
    RZERO(NV, &nm);
    

    f_meyer_energy_ver(&eng);
    f_meyer_area_ver(&area);
    f_meyer_curva_mean_ver(&curva_mean);
    f_meyer_curva_gauss_ver(&curva_gauss);

    f_meyer_laplace_ver(&lbx, &lby, &lbz);
    f_meyer_norm_ver(&nx, &ny, &nz);

    vabs(NV, FX, FY, FZ, /**/ Fm);
    MSGR("energy: %g", e0);
    MSGR("force : %g %g", FX[0], FX[NV-1]);

    vabs(NV, lbx, lby, lbz, /**/ lbm);
    vabs(NV, nx, ny, nz, /**/ nm);

    printf("x y z rxy eng Fx Fy Fz Fm area cm cg lbx lby lbz lbm nx ny nz nm\n");
    const real *queue[] = {XX, YY, ZZ, RR, eng, FX, FY, FZ, Fm, area, curva_mean, curva_gauss, lbx, lby, lbz, lbm, nx, ny, nz, nm, NULL};
    punto_fwrite(NV, queue, stdout);

    FREE(Fm);
    FREE(lbm);
    FREE(nm);
    
    
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
