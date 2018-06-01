#include <math.h>
#include <stdio.h>

#include "real.h"
#include "he/err.h"
#include "he/vec.h"
#include "he/tri.h"
#include "he/dtri.h"

/* n x (b - a)/|b - a|^2   */
static int angle0(real a[3], real b[3], real n[3],
                   real da[3]) {
    real v0, v[3], nv[3];
    vec_minus(b, a,  v);
    vec_cross(n, v, nv);
    v0 = vec_dot(v, v);
    if (v0 == 0) ERR(HE_NUM, "s = v0");    
    vec_scalar(nv, 1/v0,  da);
    return HE_OK;
}

int dtri_angle(real a[3], real b[3], real c[3],   real da[3], real db[3], real dc[3]) {
    real n[3], nda[3];
    tri_normal(b, c, a,   n);
    angle0(c, b, n,   dc);
    angle0(b, a, n,   da);
    vec_plus(dc, da,  nda);
    vec_scalar(nda, -1,   db);
    return HE_OK;
}

int dtri_cot(real a[3], real b[3], real c[3],   real da[3], real db[3], real dc[3]) {
    real ang, s, da0[3], db0[3], dc0[3];
    ang = tri_angle(a, b, c);
    s = sin(ang);
    s = -1/(s*s);
    if (s == 0) ERR(HE_NUM, "s = 0");
    dtri_angle(a, b, c,   da0, db0, dc0);
    vec_scalar(da0, s,   da);
    vec_scalar(db0, s,   db);
    vec_scalar(dc0, s,   dc);
    return HE_OK;
}
