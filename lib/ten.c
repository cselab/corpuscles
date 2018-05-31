#include "real.h"
#include "he/err.h"
#include "he/ten.h"

enum {X, Y, Z};
enum {XX, XY, XZ,   YX, YY, YZ,   ZX, ZY, ZZ};
void ten_dyadic(real a[3], real b[3], /**/ Ten T) {
    real *t;
    t = T.t;
    t[XX] = a[X]*b[X]; t[XY] = a[X]*b[Y]; t[XZ] = a[X]*b[Z];
    t[YX] = a[Y]*b[X]; t[YY] = a[Y]*b[Y]; t[YZ] = a[Y]*b[Z];
    t[ZX] = a[Z]*b[X]; t[ZY] = a[Z]*b[Y]; t[ZZ] = a[Z]*b[Z];
}

void ten_plus(Ten P, Ten R, /**/ Ten T) {
    real *p, *r, *t;
    p = P.t; r = R.t; t = T.t;
    t[XX] = p[XX] + r[XX]; t[XY] = p[XY] + r[XY]; t[XZ] = p[XZ] + r[XZ];
    t[YX] = p[YX] + r[YX]; t[YY] = p[YY] + r[YY]; t[YZ] = p[YZ] + r[YZ];
    t[ZX] = p[ZX] + r[ZX]; t[ZY] = p[ZY] + r[ZY]; t[ZZ] = p[ZZ] + r[ZZ];
}

void ten_minus(Ten P, Ten R, /**/ Ten T) {
    real *p, *r, *t;
    p = P.t; r = R.t; t = T.t;
    t[XX] = p[XX] - r[XX]; t[XY] = p[XY] - r[XY]; t[XZ] = p[XZ] - r[XZ];
    t[YX] = p[YX] - r[YX]; t[YY] = p[YY] - r[YY]; t[YZ] = p[YZ] - r[YZ];
    t[ZX] = p[ZX] - r[ZX]; t[ZY] = p[ZY] - r[ZY]; t[ZZ] = p[ZZ] - r[ZZ];
}

void ten_scalar(Ten P, real s, /**/ Ten T) {
   real *p, *r, *t;
   p = P.t; r = R.t; t = T.t;
   t[XX] = s*p[XX]; t[XY] = s*p[XY]; t[XZ] = s*p[XZ];
   t[YX] = s*p[YX]; t[YY] = s*p[YY]; t[YZ] = s*p[YZ];
   t[ZX] = s*p[ZX]; t[ZY] = s*p[ZY]; t[ZZ] = s*p[ZZ];
}

void ten_vec(Ten T, real a[3], /**/ real b[3]) {
    real *t;
    t = T.t;
    b[X] = t[XX]*a[XX] + t[XY]*a[YX] + t[ZX]*a[ZX];
    b[Y] = t[YX]*a[XY] + t[YY]*a[YY] + t[ZY]*a[ZY];
    b[Z] = t[ZX]*a[XZ] + t[ZY]*a[YZ] + t[ZZ]*a[ZZ];
}

void ten_add(Ten R, Ten T) {
   real *p, *r, *t;
   p = P.t; r = R.t; t = T.t;
   t[XX] += p[XX]; t[XY] += p[XY]; t[XZ] += p[XZ];
   t[YX] += p[YX]; t[YY] += p[YY]; t[YZ] += p[YZ];
   t[ZX] += p[ZX]; t[ZY] += p[ZY]; t[ZZ] += p[ZZ];
}
