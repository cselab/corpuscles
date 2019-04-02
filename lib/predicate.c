#include "real.h"
#include "co/err.h"
#include "co/predicate.h"

#include "predicate/main.inc"

enum {X, Y, Z};

#define CP(q, Q) \
	do { \
		Q[X] = q[X]; \
		Q[Y] = q[Y]; \
		Q[Z] = q[Z]; \
	} while (0)

int
predicate_ini(void)
{
	
	return CO_OK;
}

real
predicate_orient2d(real *a, real *b, real *c)
{
	REAL A[3], B[3], C[3];
	CP(a, A);
	CP(b, B);
	CP(c, C);
	return orient2d(A, B, C);
}


real
predicate_orient3d(real *a, real *b, real *c, real *d)
{
	REAL A[3], B[3], C[3], D[3];
	CP(a, A);
	CP(b, B);
	CP(c, C);
	CP(d, D);
	return orient3d(A, B, C, D);
}

real
predicate_incircle(real *a, real *b, real *c, real *d)
{
	REAL A[3], B[3], C[3], D[3];
	CP(a, A);
	CP(b, B);
	CP(c, C);
	CP(d, D);
	return incircle(A, B, C, D);	
}

real
predicate_insphere(real *a, real *b, real *c, real *d, real *e)
{
	REAL A[3], B[3], C[3], D[3], E[3];
	CP(a, A);
	CP(b, B);
	CP(c, C);
	CP(d, D);
	CP(e, E);
	return insphere(A, B, C, D, E);	
}

