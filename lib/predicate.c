#include "real.h"
#include "co/err.h"
#include "co/predicate.h"

#include "predicate/main.inc"

enum {
	X, Y, Z
};

#define SIGN(x) ((x) > 0 ? 1 : -1)
#define ORIENT1D(a,b) ((a) > (b) ? 1 : (a) < (b) ? -1 : 0)

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
predicate_orient2d(const real a[2], const real b[2], const real c[2])
{
	REAL A[2], B[2], C[2];
	C2(a, A);
	C2(b, B);
	C2(c, C);
	return orient2d(A, B, C);
}


real
predicate_orient3d(const real a[3], const real b[3], const real c[3], const real d[3])
{
	REAL A[3], B[3], C[3], D[3];
	C3(a, A);
	C3(b, B);
	C3(c, C);
	C3(d, D);
	return orient3d(A, B, C, D);
}

real
predicate_incircle(const real a[2], const real b[2], const real c[2], const real d[2])
{
	REAL A[2], B[2], C[2], D[2];
	C2(a, A);
	C2(b, B);
	C2(c, C);
	C2(d, D);
	return incircle(A, B, C, D);
}

real
predicate_insphere(const real a[3], const real b[3], const real c[3], const real d[3], const real e[3])
{
	REAL A[3], B[3], C[3], D[3], E[3];
	C3(a, A);
	C3(b, B);
	C3(c, C);
	C3(d, D);
	C3(e, E);
	return insphere(A, B, C, D, E);
}

int
predicate_orient3d_sas(const real a[3], const real b[3], const real c[3], const real d[3])
{
	/* TODO: points should be sorted */
	real A[2], B[2], C[2], o;
	int sign;

	o = predicate_orient3d(a, b, c, d);
	if (o != 0)
		return SIGN(o);
	else {
		sign = 1;
		/* epsilon^1/8 */
		A[X] = b[X];
		A[Y] = b[Y];
		B[X] = c[X];
		B[Y] = c[Y];
		C[X] = d[X];
		C[Y] = d[Y];
		o = predicate_orient2d(A, B, C);
		if (o != 0)
			return SIGN(o)*sign;

		/* epsilon^1/4 */
		A[X] = b[X];
		A[Y] = b[Z];
		B[X] = c[X];
		B[Y] = c[Z];
		C[X] = d[X];
		C[Y] = d[Z];
		o = predicate_orient2d(A, B, C);
		if (o != 0)
			return - SIGN(o)*sign;

		/* epsilon^1/2 */
		A[X] = b[Y];
		A[Y] = b[Z];
		B[X] = c[Y];
		B[Y] = c[Z];
		C[X] = d[Y];
		C[Y] = d[Z];
		o = predicate_orient2d(A, B, C);
		if (o != 0)
			return SIGN(o)*sign;

		/* epsilon */
		A[X] = a[X];
		A[Y] = a[Y];
		B[X] = c[X];
		B[Y] = c[Y];
		C[X] = d[X];
		C[Y] = d[Y];
		o = predicate_orient2d(A, B, C);
		if (o != 0)
			return - SIGN(o)*sign;

		/* epsilon^5/4 */
		o = ORIENT1D(c[X], d[X]);
		if (o != 0)
			return SIGN(o)*sign;

		/* epsilon^3/2 */
		o = ORIENT1D(c[Y], d[Y]);
		if (o != 0)
			return - SIGN(o)*sign;

		/* epsilon^2 */
		A[X] = a[X];
		A[Y] = a[Z];
		B[X] = c[X];
		B[Y] = c[Z];
		C[X] = d[X];
		C[Y] = d[Z];
		o = predicate_orient2d(A, B, C);
		if (o != 0)
			return SIGN(o)*sign;

		/* epsilon^5/2 */
		o = ORIENT1D(c[Z], d[Z]);
		if (o != 0)
			return SIGN(o)*sign;

		/* epsilon^4 */
		A[X] = a[Y];
		A[Y] = a[Z];
		B[X] = c[Y];
		B[Y] = c[Z];
		C[X] = d[Y];
		C[Y] = d[Z];
		o = predicate_orient2d(A, B, C);
		if (o != 0)
			return - SIGN(o)*sign;

		/* epsilon^8 */
		A[X] = a[X];
		A[Y] = a[Y];
		B[X] = b[X];
		B[Y] = b[Y];
		C[X] = d[X];
		C[Y] = d[Y];
		o = predicate_orient2d(A, B, C);
		if (o != 0)
			return SIGN(o)*sign;

		/* epsilon^33/4 */
		o = ORIENT1D(b[X], d[X]);
		if (o != 0)
			return - SIGN(o)*sign;

		/* epsilon^17/2 */
		o = ORIENT1D(b[Y], d[Y]);
		if (o != 0)
			return SIGN(o)*sign;

		/* epsilon^10 */
		o = ORIENT1D(a[X], d[X]);
		if (o != 0)
			return SIGN(o)*sign;

		/* epsilon^21/2 */
		return sign;
	}
}

int
predicate_ray(const real d[3], const real e[3],  const real a[3], const real b[3], const real c[3])
{
	int abce, abcd, adce, abde, bcde, tmp;
	const real *tmpp;

	abce = predicate_orient3d_sas(a, b, c, e);
	abcd = predicate_orient3d_sas(a, b, c, d);
	if (abce < 0 || abcd > 0) {
		tmpp = e;
		e = d;
		d = tmpp;
		tmp = abce;
		abce = abcd;
		abcd = tmp;
	}
	if (abce < 0 || abcd > 0)
		return 0;
	adce = predicate_orient3d_sas(a, d, c, e);
	if (adce < 0)
		return 0;
	abde = predicate_orient3d_sas(a, b, d, e);
	if (abde < 0)
		return 0;
	bcde = predicate_orient3d_sas(b, c, d, e);
	if (bcde < 0)
		return 0;
	return 1;
}

int
predicate_ray2(const real d[2], const real e[2],  const real a[2], const real b[2])
{
	int abe, abd, ade, dbe, t;
	const real *p;

	abe = predicate_orient2d(a, b, e);
	abd = predicate_orient2d(a, b, d);
	if (abe < 0 || abd > 0) {
		p = e;
		e = d;
		d = p;
		t = abe;
		abe = abd;
		abd = t;
	}
	if (abe < 0 || abd > 0)
		return 0;
	ade = predicate_orient2d(a, d, e);
	if (ade < 0)
		return 0;
	dbe = predicate_orient2d(d, b, e);
	if (dbe < 0)
		return 0;
	return 1;
}
