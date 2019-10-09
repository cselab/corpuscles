#include <tgmath.h>
#include <stdio.h>

#include "real.h"
#include "inc/def.h"
#include "co/err.h"
#include "co/vec.h"

enum {X, Y, Z};
#define FMT_IN   CO_REAL_IN
enum {SIZE = MAX_STRING_SIZE};

int vec_ini(real x, real y, real z, /**/ real a[3]) {
    a[X] = x; a[Y] = y; a[Z] = z;
    return CO_OK;
}

int vec_get(int i, const real x[], const real y[], const real z[], /**/ real a[3]) {
    a[X] = x[i]; a[Y] = y[i]; a[Z] = z[i];
    return CO_OK;
}

int
vec_get3(int i, int j, int k, const real x[], const real y[], const real z[], /**/ real a[3], real b[3], real c[3]) {
    vec_get(i, x, y, z, /**/ a);
    vec_get(j, x, y, z, /**/ b);
    vec_get(k, x, y, z, /**/ c);
    return CO_OK;
}

int vec_set(const real a[3], int i, /**/ real x[], real y[], real z[]) {
    x[i] = a[X]; y[i] = a[Y]; z[i] = a[Z];
    return CO_OK;
}

int vec_coord(const real a[3], /**/ real *px, real *py, real *pz) {
    *px = a[X]; *py = a[Y]; *pz = a[Z];
    return CO_OK;
}

int vec_copy(const real a[3], real b[3]) {
    b[X] = a[X]; b[Y] = a[Y]; b[Z] = a[Z];
    return CO_OK;
}

int vec_scale(real s, real a[3]) {
    a[X] *= s;
    a[Y] *= s;
    a[Z] *= s;
    return CO_OK;
}

int vec_plus(const real a[3], const real b[3], /**/ real c[3]) {
    c[X] = a[X] + b[X];
    c[Y] = a[Y] + b[Y];
    c[Z] = a[Z] + b[Z];
    return CO_OK;
}

int vec_minus(const real a[3], const real b[3], /**/ real c[3]) {
    c[X] = a[X] - b[X];
    c[Y] = a[Y] - b[Y];
    c[Z] = a[Z] - b[Z];
    return CO_OK;
}

int vec_add(const real a[3], /*io*/ real b[3]) {
    b[X] += a[X];
    b[Y] += a[Y];
    b[Z] += a[Z];
    return CO_OK;
}

int vec_sub(const real a[3], /*io*/ real b[3]) {
    b[X] -= a[X];
    b[Y] -= a[Y];
    b[Z] -= a[Z];
    return CO_OK;
}

/* y += a*x */
int vec_axpy(real a, const real x[3], /*io*/ real y[3]) {
    y[X] += a*x[X];
    y[Y] += a*x[Y];
    y[Z] += a*x[Z];
    return CO_OK;
}

int vec_neg(real x[3]) {
    x[X] = -x[X]; x[Y] = -x[Y]; x[Z] = -x[Z];
    return CO_OK;
}

int vec_linear_combination(real al, const real a[3], real be, const real b[3],
                            /**/ real c[3]) {
    c[X] = al*a[X] + be*b[X];
    c[Y] = al*a[Y] + be*b[Y];
    c[Z] = al*a[Z] + be*b[Z];
    return CO_OK;
}

int vec_scalar(const real a[3], real s, /**/ real b[3]) {
    b[X] = s*a[X]; b[Y] = s*a[Y]; b[Z] = s*a[Z];
    return CO_OK;
}

int vec_negative(const real a[3], /**/ real b[3]) {
    b[X] = -a[X]; b[Y] = -a[Y]; b[Z] = -a[Z];
    return CO_OK;
}

int vec_append(const real a[3], int i, /**/ real *x, real *y, real *z) {
   x[i] += a[X]; y[i] += a[Y]; z[i] += a[Z];
   return CO_OK;
}

int vec_substr(const real a[3], int i, /**/ real *x, real *y, real *z) {
   x[i] -= a[X]; y[i] -= a[Y]; z[i] -= a[Z];
   return CO_OK;
}

int vec_scalar_append(const real a[3], real s, int i, /**/ real *x, real *y, real *z) {
    x[i] += s*a[X]; y[i] += s*a[Y]; z[i] += s*a[Z];
    return CO_OK;
}

real vec_dot(const real a[3], const real b[3]) {
   return a[X]*b[X] + a[Y]*b[Y] + a[Z]*b[Z];
}

real vec_angle(const real a[3], const real b[3]) {
    real ang, y, x, n[3];
    vec_cross(a, b, n);
    y = vec_abs(n);
    x = vec_dot(a, b);
    ang = atan2(y, x);
    return ang;
}

int vec_cross(const real a[3], const real b[3], /**/ real c[3]) {
   c[X] = a[Y]*b[Z]-b[Y]*a[Z];
   c[Y] = b[X]*a[Z]-a[X]*b[Z];
   c[Z] = a[X]*b[Y]-b[X]*a[Y];
   return CO_OK;
}
real vec_abs(const real a[3]) { return sqrt(vec_dot(a, a)); }

real vec_cylindrical_r(const real a[3]) { return sqrt(a[X]*a[X] + a[Y]*a[Y]); }
real vec_spherical_theta(const real a[3]) {
    real r, z;
    z = a[Z];
    r = sqrt(a[X]*a[X] + a[Y]*a[Y]);
    return atan2(r, z);
}

static int small(real s) {
    const real eps = 1e-32;
    if      (s >  eps) return 0;
    else if (s < -eps) return 0;
    else               return 1;
}
int vec_norm(const real a[3], /**/ real b[3]) {
    real s;
    s = vec_abs(a);
    if (!small(s)) vec_scalar(a, 1/s, /**/ b);
    else vec_copy(a, b);
    return CO_OK;
}

int
vec_normalize(real a[3])
{
    real s;
    s = vec_abs(a);
    if (!small(s))
	vec_scale(1/s, a);
    return CO_OK;
}

int vec_project(const real a[3], const real b[3], /**/ real c[3]) {
    real n[3], s;
    vec_norm(b, /**/ n);
    s = vec_dot(a, n);
    vec_scalar(n, s, /**/ c);
    return CO_OK;
}

real vec_project_scalar(const real a[3], const real b[3]) {
    real n[3], s;
    vec_norm(b, /**/ n);
    s = vec_dot(a, n);
    return s;
}

real vec_reject_scalar(const real a[3], const real b[3]) {
    real s2, p, q;
    p = vec_project_scalar(a, b);
    s2 = vec_dot(a, a);
    q = s2 - p*p;
    return q > 0 ? sqrt(q) : 0;
}

int vec_reject(const real a[3], const real b[3], /**/ real c[3]) {
    real p[3];
    vec_project(a, b, /**/ p);
    vec_minus(a, p, /**/ c);
    return CO_OK;
}

int vec_mean3(const real a[3], const real b[3], const real c[3], /**/ real m[3]) {
    m[X] = (a[X] + b[X] + c[X])/3;
    m[Y] = (a[Y] + b[Y] + c[Y])/3;
    m[Z] = (a[Z] + b[Z] + c[Z])/3;
    return CO_OK;
}

int vec_printf(const real a[3], const char *fmt) {
    return vec_fprintf(a, stdout, fmt);
}

int vec_fprintf(const real a[3], FILE *f, const char *fmt0) {
    char fmt[SIZE];
    int r;
    r = snprintf(fmt, SIZE, "%s %s %s\n", fmt0, fmt0, fmt0);
    if (r < 0)
        ERR(CO_IO, "snprintf failed for fmt0='%s'", fmt0);

    r = fprintf(f, fmt, a[X], a[Y], a[Z]);
    return r < 0 ? CO_IO : CO_OK;
}

int vec_fprintf0(const real a[3], FILE *f, const char *fmt0) {
    char fmt[SIZE];
    int r;
    r = snprintf(fmt, SIZE, "%s %s %s", fmt0, fmt0, fmt0);
    if (r < 0)
        ERR(CO_IO, "snprintf failed for fmt0='%s'", fmt0);
    r = fprintf(f, fmt, a[X], a[Y], a[Z]);
    return r < 0 ? CO_IO : CO_OK;
}

static int nxt(const char *a, real *p) {
    return sscanf(a, FMT_IN, p) == 1;
}
int vec_argv(const char **pq[], /**/ real a[3]) {
    const char **q;
    q = *pq;
#   define NXT(d)                                                       \
    do {                                                                \
        if (*q == NULL) ERR(CO_IO, "not enough args");                  \
        if (!nxt(*q, &a[d]))                                            \
            ERR(CO_IO, "not a number '%s", *q);                         \
        q++;                                                            \
    } while (0);
    NXT(X); NXT(Y); NXT(Z);
#   undef NXT
    *pq = q;
    return CO_OK;
}

int vec_zero(real a[3]) {
    vec_ini(0, 0, 0, /**/ a);
    return CO_OK;
}

