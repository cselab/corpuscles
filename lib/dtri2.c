#include <stdio.h>
#include "real.h"
#include "co/dtri2.h"
#include "co/err.h"
#include "co/predicate.h"
#include "co/vec2.h"

int
dtri2_angle(const real a[2], const real b[2], const real c[2], real da[2],
            real db[2], real dc[2])
{
    real u[2], v[2];

    vec2_minus(a, b, u);
    vec2_minus(c, b, v);
    if (vec2_cov(u, da) != CO_OK) {
        vec2_fprintf(a, stderr);
        vec2_fprintf(b, stderr);
        vec2_fprintf(c, stderr);
        ERR(CO_NUM, "vec2_cov failed");
    }
    if (vec2_cov(v, dc) != CO_OK) {
        vec2_fprintf(a, stderr);
        vec2_fprintf(b, stderr);
        vec2_fprintf(c, stderr);
        ERR(CO_NUM, "vec2_cov failed");
    }
    vec2_neg(dc);
    vec2_plus(da, dc, db);
    vec2_neg(db);
    return CO_OK;
}

int
dtri2_angle_sup(const real a[2], const real b[2], const real c[2],
                real da[2], real db[2], real dc[2])
{
    if (dtri2_angle(a, b, c, da, db, dc) != CO_OK)
        ERR(CO_NUM, "dtri2_angle failed");
    vec2_neg(da);
    vec2_neg(db);
    vec2_neg(dc);
    return CO_OK;
}
