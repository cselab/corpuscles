#include <math.h>
#include <stdio.h>

#include "real.h"
#include "inc/def.h"
#include "co/err.h"
#include "co/vec2.h"

#define FMT   CO_REAL_IN
enum { SIZE = MAX_STRING_SIZE };

enum {
    X, Y
};

int
vec2_get(int i, const real * x, const real * y, real a[2])
{
    a[X] = x[i];
    a[Y] = y[i];
    return CO_OK;
}

real
vec2_angle(const real a[2], const real b[2])
{
    real x, y;

    x = vec2_dot(a, b);
    y = vec2_cross(a, b);
    return atan2(y, x);
}

real
vec2_abs(const real a[2])
{
    return sqrt(vec2_dot(a, a));
}

real
vec2_cross(const real a[2], const real b[2])
{
    return a[X] * b[Y] - b[X] * a[Y];
}

int
vec2_ini(real x, real y, real a[2])
{
    a[X] = x;
    a[Y] = y;
    return CO_OK;
}

int
vec2_zero(real a[2])
{
    a[X] = a[Y] = 0;
    return CO_OK;
}

int
vec2_minus(const real a[2], const real b[2], /**/ real c[2])
{
    c[X] = a[X] - b[X];
    c[Y] = a[Y] - b[Y];
    return CO_OK;
}

int
vec2_plus(const real a[2], const real b[2], /**/ real c[2])
{
    c[X] = a[X] + b[X];
    c[Y] = a[Y] + b[Y];
    return CO_OK;
}

int
vec2_neg(real a[2])
{
    a[X] = -a[X];
    a[Y] = -a[Y];
    return CO_OK;
}

int
vec2_negative(const real a[2], /**/ real b[2])
{
    b[X] = -a[X];
    b[Y] = -a[Y];
    return CO_OK;
}

real
vec2_dot(const real a[2], const real b[2])
{
    return a[X] * b[X] + a[Y] * b[Y];
}

int
vec2_copy(const real a[2], real b[2])
{
    b[X] = a[X];
    b[Y] = a[Y];
    return CO_OK;
}

int
vec2_add(const real a[3], /*io */ real b[3])
{
    b[X] += a[X];
    b[Y] += a[Y];
    return CO_OK;
}

int
vec2_scalar_add(const real a[3], real s, /*io */ real b[3])
{
    b[X] += s * a[X];
    b[Y] += s * a[Y];
    return CO_OK;
}

static int
small(real s)
{
    const real eps = 1e-32;

    if (s > eps)
        return 0;
    else if (s < -eps)
        return 0;
    else
        return 1;
}

int
vec2_scale(real s, real a[2])
{
    a[X] *= s;
    a[Y] *= s;
    return CO_OK;
}

int
vec2_scalar_append(const real a[2], real s, int i, real * x, real * y)
{
    x[i] += s * a[X];
    y[i] += s * a[Y];
    return CO_OK;
}

int
vec2_append(const real a[2], int i, real * x, real * y)
{
    x[i] += a[X];
    y[i] += a[Y];
    return CO_OK;
}

int
vec2_normalize(real a[2])
{
    real s;

    s = vec2_abs(a);
    if (!small(s))
        vec2_scale(1 / s, a);
    return CO_OK;
}

int
vec2_sub(const real a[2], /*io */ real b[2])
{
    b[X] -= a[X];
    b[Y] -= a[Y];
    return CO_OK;
}

int
vec2_cov(const real a[2], real b[2])
{
    real s;

    s = vec2_dot(a, a);
    if (s == 0)
        ERR(CO_NUM, "s == 0");
    b[X] = a[Y] / s;
    b[Y] = -a[X] / s;
    return CO_OK;
}

static int
nxt(const char *a, real * p)
{
    return sscanf(a, FMT, p) == 1;
}

int
vec2_argv(const char **pq[], /**/ real a[2])
{
    const char **q;

    q = *pq;
#define NXT(d) \
	do { \
	if (*q == NULL) ERR(CO_IO, "not enough args");  \
	if (!nxt(*q, &a[d]))  \
		ERR(CO_IO, "not a number '%s", *q);  \
		q++;  \
	} while (0);
    NXT(X);
    NXT(Y);
#undef NXT
    *pq = q;
    return CO_OK;
}

int
vec2_fprintf(const real a[2], FILE * f)
{
    if (fprintf(f, FMT " " FMT "\n", a[X], a[Y]) < 0)
        ERR(CO_IO, "fprintf failed");
    return CO_OK;
}
