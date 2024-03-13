#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <real.h>
#include <co/err.h>
/* #include <co/x.h> */
#include <co/vec.h>
#include <co/tri.h>
#include <co/memory.h>
#include <co/punto.h>

static void get3(int i, int j, int k, /**/
          real a[3], real b[3], real c[3]) {
    vec_get(i, XX, YY, ZZ, a);
    vec_get(j, XX, YY, ZZ, b);
    vec_get(k, XX, YY, ZZ, c);
}

static void mesh_ang(real *H) {
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

static void mesh_tnormal(real *HX, real *HY, real *HZ) {
    int h, n, nn;
    int i, j, k;
    real a[3], b[3], c[3], norm[3];
    for (h = 0; h < NH; h++) {
        n = nxt(h); nn = nxt(n);
        i = ver(h); j = ver(n); k = ver(nn);
        get3(i, j, k, a, b, c);
        tri_normal(a, b, c, /**/ norm);
        vec_set(norm, h, /**/ HX, HY, HZ);
    }
}

static void mesh_vnormal(real *ang, real *tx, real *ty, real *tz,  /**/ real *vx, real *vy, real *vz) {
    int h, i;
    real tt[3];
    for (h = 0; h < NH; h++) {
        i = ver(h);
        vec_get(h, tx, ty, tz, /**/ tt);
        vx[i] += ang[h]*tx[h];
        vy[i] += ang[h]*ty[h];
        vz[i] += ang[h]*tz[h];
    }    
}

static void main0() {
    real *ang;
    real *tx, *ty, *tz;  /* tri normal */
    real *vx, *vy, *vz; /* ver normal */

    RZERO(NH, &ang);
    RZERO(NH, &tx); RZERO(NH, &ty); RZERO(NH, &tz);
    RZERO(NV, &vx); RZERO(NV, &vy); RZERO(NV, &vz);

    mesh_ang(ang);
    mesh_tnormal(/**/ tx, ty, tz);
    mesh_vnormal(ang,   tx, ty, tz, /**/ vx, vy, vz);

    const real *queue[] = {XX, YY, ZZ, vx, vy, vz, NULL};
    puts("x y z nx ny nz");
    punto_fwrite(NV, queue, stdout);

    FREE(ang);
    FREE(tx); FREE(ty); FREE(tz);
    FREE(vx); FREE(vy); FREE(vz);
}

int main() {
    ini("/dev/stdin");
    main0();
    fin();
    return CO_OK;
}
