#include <math.h>
#include <stdio.h>

#include "real.h"
#include "inc/def.h"
#include "he/err.h"
#include "he/vec.h"

enum {X, Y, Z};
#define FMT_IN   XE_REAL_IN
enum {SIZE = MAX_STRING_SIZE};

int vec_ini(real x, real y, real z, /**/ real a[3]) {
    a[X] = x; a[Y] = y; a[Z] = z;
    return HE_OK;
}

int vec_get(int i, const real x[], const real y[], const real z[], /**/ real a[3]) {
    a[X] = x[i]; a[Y] = y[i]; a[Z] = z[i];
    return HE_OK;
}

int vec_set(real a[3], int i, /**/ real x[], real y[], real z[]) {
    x[i] = a[X]; y[i] = a[Y]; z[i] = a[Z];
    return HE_OK;
}

int vec_coord(real a[3], /**/ real *px, real *py, real *pz) {
    *px = a[X]; *py = a[Y]; *pz = a[Z];
    return HE_OK;
}

int vec_copy(real a[3], real b[3]) {
    b[X] = a[X]; b[Y] = a[Y]; b[Z] = a[Z];
    return HE_OK;
}

int vec_plus(real a[3], real b[3], /**/ real c[3]) {
    c[X] = a[X] + b[X];
    c[Y] = a[Y] + b[Y];
    c[Z] = a[Z] + b[Z];
    return HE_OK;
}

int vec_minus(real a[3], real b[3], /**/ real c[3]) {
    c[X] = a[X] - b[X];
    c[Y] = a[Y] - b[Y];
    c[Z] = a[Z] - b[Z];
    return HE_OK;    
}

int vec_linear_combination(real al, real a[3], real be, real b[3],
                            /**/ real c[3]) {
    c[X] = al*a[X] + be*b[X];
    c[Y] = al*a[Y] + be*b[Y];
    c[Z] = al*a[Z] + be*b[Z];
    return HE_OK;    
}

int vec_scalar(real a[3], real s, /**/ real b[3]) {
    b[X] = s*a[X]; b[Y] = s*a[Y]; b[Z] = s*a[Z];
    return HE_OK;
}

int vec_negative(real a[3], /**/ real b[3]) {
    b[X] = -a[X]; b[Y] = -a[Y]; b[Z] = -a[Z];
    return HE_OK;
}

int vec_append(real a[3], int i, /**/ real *x, real *y, real *z) {
   x[i] += a[X]; y[i] += a[Y]; z[i] += a[Z];
   return HE_OK;
}

int vec_substr(real a[3], int i, /**/ real *x, real *y, real *z) {
   x[i] -= a[X]; y[i] -= a[Y]; z[i] -= a[Z];
   return HE_OK;
}

int vec_scalar_append(real a[3], real s, int i, /**/ real *x, real *y, real *z) {
    x[i] += s*a[X]; y[i] += s*a[Y]; z[i] += s*a[Z];
    return HE_OK;
}

real vec_dot(real a[3], real b[3]) {
   return a[X]*b[X] + a[Y]*b[Y] + a[Z]*b[Z];
}

real vec_angle(real a[3], real b[3]) {
    real ang, y, x, n[3];
    vec_cross(a, b, n);
    y = vec_abs(n);
    x = vec_dot(a, b);
    ang = atan2(y, x);
    if (ang < 0) ang = -ang;
    return ang;
}

int vec_cross(real a[3], real b[3], /**/ real c[3]) {
   c[X] = a[Y]*b[Z]-b[Y]*a[Z];
   c[Y] = b[X]*a[Z]-a[X]*b[Z];
   c[Z] = a[X]*b[Y]-b[X]*a[Y];
   return HE_OK;
}
real vec_abs(real a[3]) { return sqrt(vec_dot(a, a)); }

real vec_cylindrical_r(real a[3]) { return sqrt(a[X]*a[X] + a[Y]*a[Y]); }
real vec_spherical_theta(real a[3]) {
    real r, z;
    z = a[Z];
    r = sqrt(a[X]*a[X] + a[Y]*a[Y]);
    return atan2(r, z);
}

static int small(real s) {
    const real eps = 1e-12;
    if      (s >  eps) return 0;
    else if (s < -eps) return 0;
    else               return 1;
}
int vec_norm(real a[3], /**/ real b[3]) {
    real s;
    s = vec_abs(a);
    if (!small(s)) vec_scalar(a, 1/s, /**/ b);
    else vec_copy(a, b);
    return HE_OK;
}

int vec_printf(real a[3], const char *fmt) {
    return vec_fprintf(a, stdout, fmt);
}

int vec_fprintf(real a[3], FILE *f, const char *fmt0) {
    char fmt[SIZE];
    int r;
    r = snprintf(fmt, SIZE, "%s %s %s\n", fmt0, fmt0, fmt0);
    if (r < 0)
        ERR(HE_IO, "snprintf failed for fmt0='%s'", fmt0);

    r = fprintf(f, fmt, a[X], a[Y], a[Z]);
    return r < 0 ? HE_IO : HE_OK;
}

static int nxt(const char *a, real *p) {
    return sscanf(a, FMT_IN, p) == 1;
}
int vec_argv(const char **pq[], /**/ real a[3]) {
    const char **q;
    q = *pq;
#   define NXT(d)                                                       \
    do {                                                                \
        if (*q == NULL) ERR(HE_IO, "not enough args");                  \
        if (!nxt(*q, &a[d]))                                            \
            ERR(HE_IO, "not a number '%s", *q);                         \
        q++;                                                            \
    } while (0);
    NXT(X); NXT(Y); NXT(Z);
#   undef NXT
    *pq = q;
    return HE_OK;
}

int vec_zero(real a[3]) {
    vec_ini(0, 0, 0, /**/ a);
    return HE_OK;
}
