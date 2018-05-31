#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <real.h>
#include <he/err.h>
#include <he/x.h>
#include <he/vec.h>
#include <he/tri.h>
#include <he/memory.h>
#include <he/punto.h>

static void get3(int i, int j, int k, /**/
          real a[3], real b[3], real c[3]) {
    vec_get(i, XX, YY, ZZ, a);
    vec_get(j, XX, YY, ZZ, b);
    vec_get(k, XX, YY, ZZ, c);
}

static void mean_ang(real *H) {
    int h, n, nn;
    int i, j, k;
    real a[3], b[3], c[3];    
    for (h = 0; h < NH; h++) {
        n = nxt(h); nn = nxt(n);
        i = ver(h); j = ver(n); k = ver(nn);
        get3(i, j, k, a, b, c);
        H[h] = tri_angle(a, b, c);
    }
}

static void main0() {
    real *ang;
    real *tx, *ty, *tz;  /* tri normal */
    real *vx, *vy, *vz; /* ver normal */
    
    RZERO(NH, &ang);
    RZERO(NH, &tx); RZERO(NH, &ty); RZERO(NH, &tz);
    RZERO(NV, &vx); RZERO(NV, &vy); RZERO(NV, &vz);

    mean_ang(ang);

    FREE(ang);
    FREE(tx); FREE(ty); FREE(tz);
    FREE(vx); FREE(vy); FREE(vz);
}

int main() {
    ini("/dev/stdin");
    main0();
    fin();
    return HE_OK;
}
