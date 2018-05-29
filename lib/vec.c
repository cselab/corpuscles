#include <math.h>
#include <stdio.h>

#include "he/vec.h"

void vec_ini(double x, double y, double z, /**/ double a[3]) {
    enum {X, Y, Z};
    a[X] = x; a[Y] = y; a[Z] = z;
}

void vec_coord(double a[3], /**/ double *px, double *py, double *pz) {
    enum {X, Y, Z};
    *px = a[X]; *py = a[Y]; *pz = a[Z];
}

void vec_copy(double a[3], double b[3]) {
    enum {X, Y, Z};
    b[X] = a[X]; b[Y] = a[Y]; b[Z] = a[Z];
}

void vec_plus(double a[3], double b[3], /**/ double c[3]) {
    enum {X, Y, Z};
    c[X] = a[X] + b[X];
    c[Y] = a[Y] + b[Y];
    c[Z] = a[Z] + b[Z];
}

void vec_minus(double a[3], double b[3], /**/ double c[3]) {
    enum {X, Y, Z};
    c[X] = a[X] - b[X];
    c[Y] = a[Y] - b[Y];
    c[Z] = a[Z] - b[Z];
}

void vec_scalar(double a[3], double s, double b[3]) {
   enum {X, Y, Z};
    b[X] = s*a[X]; b[Y] = s*a[Y]; b[Z] = s*a[Z];
}

void vec_append(double a[3], /**/ double *x, double *y, double *z) {
   enum {X, Y, Z};
   *x += a[X]; *y += a[Y]; *z += a[Z];
}

void vec_append_direction(double a[3], double s, /**/ double *x, double *y, double *z) {
    double e[3], b[3];
    vec_norm(a, /**/ e);
    vec_scalar(e, s, /**/ b);
    vec_append(b, /**/ x, y, z);
}

double vec_dot(double a[3], double b[3]) {
   enum {X, Y, Z};
   return a[X]*b[X] + a[Y]*b[Y] + a[Z]*b[Z];
}

void vec_cross(double a[3], double b[3], /**/ double c[3]) {
   enum {X, Y, Z};    
   c[X] = a[Y]*b[Z]-b[Y]*a[Z];
   c[Y] = b[X]*a[Z]-a[X]*b[Z];
   c[Z] = a[X]*b[Y]-b[X]*a[Y];
}

double vec_abs(double a[3]) { return sqrt(vec_dot(a, a)); }

static int small(double s) {
    const double eps = 1e-12;
    if      (s >  eps) return 0;
    else if (s < -eps) return 0;
    else               return 1;
}
void vec_norm(double a[3], /**/ double b[3]) {
    double s;
    s = vec_abs(a);
    if (!small(s)) vec_scalar(a, 1/s, /**/ b);
    else vec_copy(a, b);
}

int vec_printf(double a[3], FILE *f, const char *fmt) {
    enum {X, Y, Z};
    int n;
    n = fprintf(f, fmt, a[X], a[Y], a[Z]);
    return n == 3;
}
