#include <math.h>
#include <stdio.h>

#include "real.h"
#include "he/err.h"
#include "he/vec.h"
#include "he/tri.h"
#include "he/dtri.h"
#include "he/ten.h"

/* n x (b - a)/|b - a|^2   */
static int angle0(const real a[3], const real b[3], const real n[3], /**/ real da[3]) {
    real v0, v[3], nv[3];
    vec_minus(b, a,  v);
    vec_cross(n, v, nv);
    v0 = vec_dot(v, v);
    if (v0 == 0) ERR(HE_NUM, "s = v0");
    vec_scalar(nv, 1/v0,  da);
    return HE_OK;
}

int dtri_angle(const real a[3], const real b[3], const real c[3],   real da[3], real db[3], real dc[3]) {
    real n[3], nda[3];
    tri_normal(b, c, a,   n);
    angle0(c, b, n,   dc);
    angle0(b, a, n,   da);
    vec_plus(dc, da,  nda);
    vec_scalar(nda, -1,   db);
    return HE_OK;
}

int dtri_cot(const real a[3], const real b[3], const real c[3],   real da[3], real db[3], real dc[3]) {
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

int dtri_area(const real a[3], const real b[3], const real c[3],  /**/ real da[3], real db[3], real dc[3]) {
    real n[3], n2[3], ab[3], bc[3], ca[3];
    tri_normal(a, b, c,   n);
    vec_scalar(n, 0.5,   n2);
    tri_edg(a, b, c,   ab, bc, ca);
    vec_cross(n2, bc,   da);
    vec_cross(n2, ca,   db);
    vec_cross(n2, ab,   dc);
    return HE_OK;
}

static int dvolume(const real a[3], const real b[3], const real c[3], /**/ real dd[3]) {
    real n[3], area;
    tri_normal(a, b, c, /**/ n);
    area = tri_area(a, b, c);
    vec_scalar(n, area/3, /**/ dd);
    return HE_OK;
}
int dtri_volume(const real a[3], const  real b[3], const real c[3], /**/ real da[3], real db[3], real dc[3]) {
    real z[3];
    vec_zero(z);
    dvolume(z, a, b, /**/ dc);
    dvolume(z, b, c, /**/ da);
    dvolume(z, c, a, /**/ db);
    return HE_OK;
}

int dtri_normal(const real a[3], const real b[3], const real c[3], /**/ Ten *x, Ten *y, Ten *z) {
    real n[3], A, u[3], v[3], w[3];
    tri_normal(a, b, c,   n);
    A = tri_area(a, b, c);
    /* TODO */
    return HE_OK;
}
