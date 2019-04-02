#include "real.h"
#include "co/err.h"
#include "co/predicate.h"

#include "predicate/main.inc"

enum {
	X, Y, Z
};

#define C3(q, Q) \
	do { \
		Q[X] = q[X]; \
		Q[Y] = q[Y]; \
		Q[Z] = q[Z]; \
	} while (0)

#define C2(q, Q) \
	do { \
		Q[X] = q[X]; \
		Q[Y] = q[Y]; \
	} while (0)

int
predicate_ini(void)
{
	exactinit();
	return CO_OK;
}

real
predicate_orient2d(real *a, real *b, real *c)
{
	REAL A[2], B[2], C[2];
	C2(a, A);
	C2(b, B);
	C2(c, C);
	return orient2d(A, B, C);
}


real
predicate_orient3d(real *a, real *b, real *c, real *d)
{
	REAL A[3], B[3], C[3], D[3];
	C3(a, A);
	C3(b, B);
	C3(c, C);
	C3(d, D);
	return orient3d(A, B, C, D);
}

real
predicate_incircle(real *a, real *b, real *c, real *d)
{
	REAL A[2], B[2], C[2], D[2];
	C2(a, A);
	C2(b, B);
	C2(c, C);
	C2(d, D);
	return incircle(A, B, C, D);
}

real
predicate_insphere(real *a, real *b, real *c, real *d, real *e)
{
	REAL A[3], B[3], C[3], D[3], E[3];
	C3(a, A);
	C3(b, B);
	C3(c, C);
	C3(d, D);
	C3(e, E);
	return insphere(A, B, C, D, E);
}

