#include <tgmath.h>
#include <stdio.h>

#include "real.h"
#include "inc/def.h"
#include "co/err.h"
#include "co/vec2.h"

#define FMT   CO_REAL_IN
enum {SIZE = MAX_STRING_SIZE};

enum
{
	X, Y
};

int
vec2_get(int i, const real *x, const real *y, real a[2])
{
	a[X] = x[i];
	a[Y] = y[i];
	return CO_OK;
}

real
vec2_abs(const real a[2])
{
	return sqrt(vec2_dot(a, a));
}

int
vec2_ini(real x, real y, real a[3])
{
	a[X] = x;
	a[Y] = y;
	return CO_OK;
}

int
vec2_minus(const real a[2], const real b[2], /**/ real c[2])
{
	c[X] = a[X] - b[X];
	c[Y] = a[Y] - b[Y];
	return CO_OK;
}

real
vec2_dot(const real a[2], const real b[2])
{
	return a[X]*b[X] + a[Y]*b[Y];
}

int
vec2_copy(const real a[2], real b[2])
{
	b[X] = a[X];
	b[Y] = a[Y];
	return CO_OK;
}

int
vec2_add(const real a[3], /*io*/ real b[3])
{
	b[X] += a[X];
	b[Y] += a[Y];
	return CO_OK;
}

int
vec2_sub(const real a[3], /*io*/ real b[3])
{
	b[X] -= a[X];
	b[Y] -= a[Y];
	return CO_OK;
}

static int
nxt(const char *a, real *p)
{
	return sscanf(a, FMT, p) == 1;
}
int
vec2_argv(const char **pq[], /**/ real a[3])
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
	NXT(X); NXT(Y);
# undef NXT
	*pq = q;
	return CO_OK;
}

int
vec2_fprintf(const real a[2], FILE *f)
{
	if (fprintf(f, FMT " " FMT "\n", a[X], a[Y]) < 0)
		ERR(CO_IO, "fprintf failed");
	return CO_OK;
}
