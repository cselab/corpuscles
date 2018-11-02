#include <stdio.h>

#include "real.h"

#include "he/vec.h"
#include "he/tri.h"
#include "he/err.h"
#include "he/macro.h"
#include "he/constant_strain/2d.h"

static const real EPS = 1e-8;

static int small(const real a[3]) { return vec_abs(a) < EPS; }

static int assert_force_2d(real ax, real ay,
                           real bx, real by,
                           real cx, real cy,
                           real dvx, real dvy,
                           real dux, real duy,
                           real dwx, real dwy) {
    /* check total force and  torque */
    return 1;
}

static int assert_force_3d(const real a[3], const real b[3], const real c[3],
                           const real da[3], const real db[3], const real dc[3]) {
    /* check total force and  torque */
    real center[3], m[3], f[3], t[3], ma[3], mb[3], mc[3];
    real ta[3], tb[3], tc[3];
    tri_center(a, b, c, /**/ m);
    vec_minus(a, m, /**/ ma);
    vec_minus(b, m, /**/ mb);
    vec_minus(c, m, /**/ mc);
    vec_mean3(da, db, dc, /**/ f);

    vec_cross(ma, da, /**/ ta);
    vec_cross(mb, db, /**/ tb);
    vec_cross(mc, dc, /**/ tc);
    vec_mean3(ta, tb, tc, /**/ t);

    if (!small(f) || !small(t))  {
        MSG("bad 3d triangle in constant_strain");
        MSG("a, b, c, f, t:");
        vec_fprintf(a, stderr, "%.16g");
        vec_fprintf(b, stderr, "%.16g");
        vec_fprintf(c, stderr, "%.16g");
        vec_fprintf(f, stderr, "%.16g");
        vec_fprintf(t, stderr, "%.16g");
        return 0;
    } else
        return 1;
}

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
    if (!assert_force_2d(ax + vx, ay + vy,
                         bx + ux, vy + uy,
                         cx + wx, cy + wy,
                         dvx, dvy, dux, duy, dwx, dwy))
        ERR(HE_NUM, "bad 2d forces in triangle");

    tri_2to3(a, b, c, /**/ ex, ey);
    vec_linear_combination(dvx, ex,  dvy, ey, /**/ da);
    vec_linear_combination(dux, ex,  duy, ey, /**/ db);
    vec_linear_combination(dwx, ex,  dwy, ey, /**/ dc);
    if (!assert_force_3d(a, b, c, da, db, dc))
        ERR(HE_NUM, "bad 3d forces in triangle");
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
