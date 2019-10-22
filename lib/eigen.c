#include <stdio.h>

#include "real.h"

#include "co/eigen.h"
#include "co/err.h"
#include "co/he.h"
#include "co/memory.h"
#include "co/sum.h"
#include "co/ten.h"
#include "co/tri.h"
#include "co/vec.h"
#include <alg/eig.h>

#define T    Eigen

struct T {
	int n;
	real v[3 * 3];
};


static real
sum(int n, real * a)
{
	return (he_sum_array(n, a));
}

int
eigen_ini(He * he, T ** pq)
{
	T *q;

	MALLOC(1, &q);
	q->n = he_nv(he);
	*pq = q;
	return CO_OK;
}


int
eigen_fin(T * q)
{
	FREE(q);
	return CO_OK;
}


static int
moment(int n, const real * xx, const real * yy, const real * zz,
	   /**/ real m[6])
{
	/* moment of inertia tensor */
	int i;
	real x, y, z;

	enum {
		XX,
		XY,
		XZ,
		YY,
		YZ,
		ZZ
	};

	m[XX] = m[XY] = m[XZ] = m[YY] = m[YZ] = m[ZZ] = 0;
	for (i = 0; i < n; i++) {
		x = xx[i];
		y = yy[i];
		z = zz[i];
		m[XX] += y * y + z * z;
		m[XY] += -x * y;
		m[XZ] += -x * z;
		m[YY] += x * x + z * z;
		m[YZ] += -y * z;
		m[ZZ] += x * x + y * y;
	}
	return CO_OK;
}


static int
to_cm(int n, /**/ real * xx, real * yy, real * zz)
{
	int i;
	real x, y, z;

	x = sum(n, xx) / n;
	y = sum(n, yy) / n;
	z = sum(n, zz) / n;
	for (i = 0; i < n; i++) {
		xx[i] -= x;
		yy[i] -= y;
		zz[i] -= z;
	}
	return CO_OK;
}

int
eigen_vector(T * q, real * x, real * y, real * z, /**/ real * ev)
{
	int n, i;
	real m[6];
	real *v;

	n = q->n;
	v = q->v;
	to_cm(n, /**/ x, y, z);
	moment(n, x, y, z, /**/ m);
	alg_eig_vectors(m, v);
	for (i = 0; i < 9; i++)
		ev[i] = v[i];
	return CO_OK;
}

static int
moment_surface(He * he, const real * xx, const real * yy, const real * zz,
			   /**/ real * m)
{
	enum { X, Y, Z };
	enum {
		XX,
		XY,
		XZ,
		YY,
		YZ,
		ZZ
	};
	real a[3], b[3], c[3], r[3], area;
	real x, y, z;
	int nt, t, i, j, k;

	m[XX] = m[XY] = m[XZ] = m[YY] = m[YZ] = m[ZZ] = 0;
	nt = he_nt(he);
	for (t = 0; t < nt; t++) {
		he_tri_ijk(he, t, &i, &j, &k);
		vec_get3(i, j, k, xx, yy, zz, a, b, c);
		area = tri_area(a, b, c);
		tri_center(a, b, c, r);
		x = r[X];
		y = r[Y];
		z = r[Z];
		m[XX] += (y * y + z * z) * area;
		m[XY] += (-x * y) * area;
		m[XZ] += (-x * z) * area;
		m[YY] += (x * x + z * z) * area;
		m[YZ] += (-y * z) * area;
		m[ZZ] += (x * x + y * y) * area;
	}
	return CO_OK;
}

static int
moment_tri(He * he, const real * xx, const real * yy, const real * zz,
		   /**/ real * m)
{
	enum { X, Y, Z };
	enum {
		XX,
		XY,
		XZ,
		YY,
		YZ,
		ZZ
	};
	real a[3], b[3], c[3], area;
	real x, y, z;
	int nt, t, i, j, k;
	real uu, uv, uw, vv, vw, ww;

	m[XX] = m[XY] = m[XZ] = m[YY] = m[YZ] = m[ZZ] = 0;
	nt = he_nt(he);
	for (t = 0; t < nt; t++) {
		he_tri_ijk(he, t, &i, &j, &k);
		vec_get3(i, j, k, xx, yy, zz, a, b, c);
		tri_moment(a, b, c, &uu, &uv, &uw, &vv, &vw, &ww);
		area = tri_area(a, b, c);
		m[XX] += (vv + ww) * area;
		m[YY] += (uu + ww) * area;
		m[ZZ] += (uu + vv) * area;
		m[XY] += -uv * area;
		m[XZ] += -uw * area;
		m[YZ] += -vw * area;
	}
	return CO_OK;
}

int
eigen_vector_surface(T * q, He * he, real * x, real * y, real * z,
					 /**/ real * ev)
{
	int n, i;
	real m[6];
	real *v;

	n = q->n;
	v = q->v;
	to_cm(n, /**/ x, y, z);
	moment_surface(he, x, y, z, /**/ m);
	alg_eig_vectors(m, v);
	for (i = 0; i < 9; i++)
		ev[i] = v[i];
	return CO_OK;
}

int
eigen_vector_tri(T * q, He * he, real * x, real * y, real * z,
				 /**/ real * ev)
{
	int n, i;
	real m[6];
	real *v;

	n = q->n;
	v = q->v;
	to_cm(n, /**/ x, y, z);
	moment_tri(he, x, y, z, /**/ m);
	alg_eig_vectors(m, v);
	for (i = 0; i < 9; i++)
		ev[i] = v[i];
	return CO_OK;
}
