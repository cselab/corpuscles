#include <math.h>
#include <stdio.h>

#include "real.h"
#include "he/vec.h"

void vec_ini(real x, real y, real z, /**/ real a[3]) {
    enum {X, Y, Z};
    a[X] = x; a[Y] = y; a[Z] = z;
}

void vec_get(int i, real x[], real y[], real z[], /**/ real a[3]) {
    enum {X, Y, Z};
    a[X] = x[i]; a[Y] = y[i]; a[Z] = z[i];
}

void vec_coord(real a[3], /**/ real *px, real *py, real *pz) {
    enum {X, Y, Z};
    *px = a[X]; *py = a[Y]; *pz = a[Z];
}

void vec_copy(real a[3], real b[3]) {
    enum {X, Y, Z};
    b[X] = a[X]; b[Y] = a[Y]; b[Z] = a[Z];
}

void vec_plus(real a[3], real b[3], /**/ real c[3]) {
    enum {X, Y, Z};
    c[X] = a[X] + b[X];
    c[Y] = a[Y] + b[Y];
    c[Z] = a[Z] + b[Z];
}

void vec_minus(real a[3], real b[3], /**/ real c[3]) {
    enum {X, Y, Z};
    c[X] = a[X] - b[X];
    c[Y] = a[Y] - b[Y];
    c[Z] = a[Z] - b[Z];
}

void vec_scalar(real a[3], real s, real b[3]) {
   enum {X, Y, Z};
    b[X] = s*a[X]; b[Y] = s*a[Y]; b[Z] = s*a[Z];
}

void vec_append(real a[3], /**/ real *x, real *y, real *z) {
   enum {X, Y, Z};
   *x += a[X]; *y += a[Y]; *z += a[Z];
}

real vec_dot(real a[3], real b[3]) {
   enum {X, Y, Z};
   return a[X]*b[X] + a[Y]*b[Y] + a[Z]*b[Z];
}

static real atan2_pi(real y, real x) {
    return fabs(atan2(y, x));
}
real vec_angle(real a[3], real b[3]) {
    real y, x, n[3];
    vec_cross(a, b, n);
    y = vec_abs(n);
    x = vec_dot(a, b);
    return atan2_pi(y, x);
}

void vec_cross(real a[3], real b[3], /**/ real c[3]) {
   enum {X, Y, Z};    
   c[X] = a[Y]*b[Z]-b[Y]*a[Z];
   c[Y] = b[X]*a[Z]-a[X]*b[Z];
   c[Z] = a[X]*b[Y]-b[X]*a[Y];
}
real vec_abs(real a[3]) { return sqrt(vec_dot(a, a)); }

real vec_cylindrical_r(real a[3]) {
    enum {X, Y};
    return sqrt(a[X]*a[X] + a[Y]*a[Y]);
}

static int small(real s) {
    const real eps = 1e-12;
    if      (s >  eps) return 0;
    else if (s < -eps) return 0;
    else               return 1;
}
void vec_norm(real a[3], /**/ real b[3]) {
    real s;
    s = vec_abs(a);
    if (!small(s)) vec_scalar(a, 1/s, /**/ b);
    else vec_copy(a, b);
}

int vec_printf(real a[3], FILE *f, const char *fmt) {
    enum {X, Y, Z};
    int n;
    n = fprintf(f, fmt, a[X], a[Y], a[Z]);
    return n == 3;
}
