#include <stdio.h>
#include <math.h>

#include <real.h>
#include <co/f/kantor.h>
#include <co/memory.h>
#include <co/punto.h>
/* #include <co/x.h> */

static real *FX, *FY, *FZ;

static void vabs(int n, real *x, real *y, real *z, /**/ real *r) {
  /*Given n vectors with x, y, z components,
    calculate the absolute value/Euclidean length for each vector.*/
  
    int i;
    
    for (i = 0; i < n; i++)
        r[i] = sqrt(x[i]*x[i] + y[i]*y[i] + z[i]*z[i]);
    
}

static void main0() {
  //real *queue[] = {XX, YY, ZZ, FX, FY, FZ, NULL};

    
    fprintf(stderr, "eng: %g\n", f_kantor_energy(XX, YY, ZZ));
    f_kantor_force(XX, YY, ZZ,   FX, FY, FZ);

    real *fm;;
    RZERO(NV, &fm);
    vabs(NV, FX, FY, FZ, /**/ fm);
    const real *queue[] = {TH, RR, ZZ, fm, FX, FY, FZ,  NULL};
    printf("#1 azimuth angle; 2 axis dist; 3 zz; 4 |F|; 5 FX; 6 FY; 7 FZ \n");

    punto_fwrite(NV, queue, stdout);
}

int main() {
    real K, theta0;
    K = 1; theta0 = 0;
    ini("/dev/stdin");
    f_kantor_ini(K, theta0);

    RZERO(NV, &FX); RZERO(NV, &FY); RZERO(NV, &FZ);
    main0();

    FREE(FX); FREE(FY); FREE(FZ);
    f_kantor_fin();
    fin();
    return 0;
}
