#include <stdio.h>

#include "real.h"

#include "he/vec.h"
#include "he/tri.h"
#include "he/err.h"
#include "he/macro.h"
#include "he/constant_strain/2d.h"

int constant_strain_force(void *param,
                          real (*F1)(void*, real, real), real (*F2)(void*, real, real),
                          const real a0[3], const real b0[3], const real c0[3],
                          const real a[3], const real b[3], const real c[3], /**/
                          real da_tot[3], real db_tot[3], real dc_tot[3]) {
    real da[3], db[3], dc[3];
    real ax, ay, bx, by, cx, cy, vx, vy, ux, uy, wx, wy;
    real dvx, dvy, dux, duy, dwx, dwy;
    real area, I1, I2;
    real ex[3], ey[3];

    tri_3to2(a0, b0, c0, /**/ &bx, &by, &cx, &cy);
    tri_3to2(a, b, c, /**/ &ux, &uy, &wx, &wy);
    ux -= bx; uy -= by; /* displace */
    wx -= cx; wy -= cy;

    ax = ay = vx = vy = 0;
    constant_strain_2d(param, F1, F2,
                       ax, ay, bx, by, cx, cy,
                       vx, vy, ux, uy, wx, wy,
                       &dvx, &dvy, &dux, &duy, &dwx, &dwy,
                       &I1, &I2, &area);

    tri_2to3(a, b, c, /**/ ex, ey);
    vec_linear_combination(dvx, ex,  dvy, ey, /**/ da);
    vec_linear_combination(dux, ex,  duy, ey, /**/ db);
    vec_linear_combination(dwx, ex,  dwy, ey, /**/ dc);

    vec_scalar(da, area, /**/ da_tot);
    vec_scalar(db, area, /**/ db_tot);
    vec_scalar(dc, area, /**/ dc_tot);

    return HE_OK;
}

static real Dummy(__UNUSED void *param, __UNUSED real I1, __UNUSED real I2) { return 0; }
int constant_strain_energy(void *param, real (*F)(void*, real, real),
                           const real a0[3], const real b0[3], const real c0[3],
                           const real a[3], const real b[3], const real c[3],
                           real *p_eng, real *p_deng) {
    real ax, ay, bx, by, cx, cy, vx, vy, ux, uy, wx, wy;
    real I1, I2, A, eng, deng;

    tri_3to2(a0, b0, c0, /**/ &bx, &by, &cx, &cy);
    tri_3to2(a, b, c, /**/ &ux, &uy, &wx, &wy);
    ux -= bx; uy -= by; /* displace */
    wx -= cx; wy -= cy;

    ax = ay = vx = vy = 0;
    constant_strain_2d(param, Dummy, Dummy,
                       ax, ay, bx, by, cx, cy,
                       vx, vy, ux, uy, wx, wy,
                       NULL, NULL, NULL, NULL, NULL, NULL,
                       &I1, &I2, &A);

    deng = F(param, I1, I2);
    eng = deng * A;

    *p_eng = eng;
    *p_deng = deng;

    return HE_OK;
}
