#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>

#include "real.h"

#include "he/vec.h"
#include "he/tri.h"
#include "he/err.h"
#include "he/macro.h"
#include "he/dtri.h"
#include "he/strain/2d.h"

#define FMT HE_REAL_OUT

static const real EPS = 1e-8;

static int env_area(void) {
    enum {UNSET, YES, NO};
    static int f = UNSET;
    if (f == UNSET)
        f = getenv("AREA") ? YES : NO;
    return f == YES;
}

static int small_v(const real a[3]) { return vec_abs(a) < EPS; }
static int small(real a) { return fabs(a) < EPS; }

static int mean(real ax, real ay, real bx, real by, real cx, real cy, /**/ real *mx, real *my) {
    *mx = (ax + bx + cx)/3;
    *my = (ay + by + cy)/3;
    return HE_OK;
}
static int minus(real ax, real ay, real bx, real by, /**/ real *cx, real *cy) {
    *cx = ax - bx;
    *cy = ay - by;
    return HE_OK;
}
static real cross(real ax, real ay, real bx, real by) {
    return ax*by - ay*bx;
}

static int assert_force_3d(const real a[3], const real b[3], const real c[3],
                           const real da[3], const real db[3], const real dc[3]) {
    /* check total force and  torque */
    real m[3], f[3], t[3], ma[3], mb[3], mc[3];
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

    if (!small_v(f) || !small_v(t))  {
        MSG("bad 3d triangle in strain");
        MSG("a, b, c, f, t:");
        vec_fprintf(a, stderr, FMT);
        vec_fprintf(b, stderr, FMT);
        vec_fprintf(c, stderr, FMT);

        vec_fprintf(da, stderr, FMT);
        vec_fprintf(db, stderr, FMT);
        vec_fprintf(dc, stderr, FMT);

        vec_fprintf(f, stderr, FMT);
        vec_fprintf(t, stderr, FMT);
        return 0;
    } else
        return 1;
}

int strain_force_3d(void *param,
                    real (*F)(void*, real, real), real (*F1)(void*, real, real), real (*F2)(void*, real, real),
                    const real a0[3], const real b0[3], const real c0[3],
                    const real a[3], const real b[3], const real c[3], /**/
                    real da_tot[3], real db_tot[3], real dc_tot[3]) {
    real da[3], db[3], dc[3];
    real bx, cx, cy, ux, wx, wy;
    real dvx, dvy, dux, duy, dwx, dwy;
    real area, I1, I2;
    real ex[3], ey[3], deng;

    tri_3to2(a0, b0, c0, /**/ &bx, &cx, &cy);
    tri_3to2(a, b, c, /**/ &ux, &wx, &wy);

    strain_2d(param, F1, F2,
              bx, cx, cy,
              ux, wx, wy,
              &dvx, &dvy, &dux, &duy, &dwx, &dwy,
              &I1, &I2, &area);
    if (!small(dvx + dux + dwx))
        ERR(HE_NUM,
            "2d force fails: " FMT " " FMT " " FMT,
            dvx, dux, dwx);

    if (!small(dvy + duy + dwy))
        ERR(HE_NUM,
            "2d force fails: " FMT " " FMT " " FMT,
            dvy, duy, dwy);

    tri_2to3(a, b, c, /**/ ex, ey);
    vec_linear_combination(dvx, ex,  dvy, ey, /**/ da);
    vec_linear_combination(dux, ex,  duy, ey, /**/ db);
    vec_linear_combination(dwx, ex,  dwy, ey, /**/ dc);
    if (!assert_force_3d(a, b, c, da, db, dc))
        ERR(HE_NUM, "bad 3d forces in triangle");
    area = fabs(area);
    vec_scalar(da, area, /**/ da_tot);
    vec_scalar(db, area, /**/ db_tot);
    vec_scalar(dc, area, /**/ dc_tot);
    if (env_area()) {
        deng = F(param, I1, I2);
        dtri_area(a, b, c, da, db, dc);
        vec_axpy(deng, da, da_tot);
        vec_axpy(deng, db, db_tot);
        vec_axpy(deng, dc, dc_tot);
    }
    return HE_OK;
}

static real Dummy(__UNUSED void *param, __UNUSED real I1, __UNUSED real I2) { return 0; }
int strain_energy_3d(void *param, real (*F)(void*, real, real),
                     const real a0[3], const real b0[3], const real c0[3],
                     const real a[3], const real b[3], const real c[3],
                     real *p_eng, real *p_deng) {
    real bx, _by, cx, cy, ux, wx, wy;
    real I1, I2, A, eng, deng;

    tri_3to2(a0, b0, c0, /**/ &bx, &cx, &cy);
    tri_3to2(a, b, c, /**/ &ux, &wx, &wy);

    strain_2d(param, Dummy, Dummy,
              bx, cx, cy,
              ux, wx, wy,
              NULL, NULL, NULL, NULL, NULL, NULL,
              &I1, &I2, &A);

    deng = F(param, I1, I2);
    eng = deng * fabs(A);

    *p_eng = eng;
    *p_deng = deng;

    return HE_OK;
}

int strain_invariants(const real a0[3], const real b0[3], const real c0[3],
                      const real a[3], const real b[3], const real c[3],
                      real *I1, real *I2) {
    real bx, _by, cx, cy, ux, wx, wy;

    tri_3to2(a0, b0, c0, /**/ &bx, &cx, &cy);
    tri_3to2(a, b, c, /**/ &ux, &wx, &wy);

    return strain_2d(NULL, Dummy, Dummy,
                     bx, cx, cy,
                     ux, wx, wy,
                     NULL, NULL, NULL, NULL, NULL, NULL,
                     I1, I2, NULL);
}
