#include <tgmath.h>
#include <stdio.h>

#include "real.h"
#include "co/err.h"
#include "co/vec.h"
#include "co/tri.h"

#include "co/dih.h"
static const real pi = 3.141592653589793115997964;

real
dih_angle(const real a[3], const real b[3], const real c[3],
		  const real d[3])
{
	real ang, ang_s;

	ang_s = dih_angle_sup(a, b, c, d);
	if (ang_s >= 0)
		ang = pi - ang_s;
	else
		ang = -pi - ang_s;
	return ang;
}

real
dih_angle_sup(const real a[3], const real b[3], const real c[3],
			  const real d[3])
{
	/*supplementary angle of dihedral */
	real x, y, ang, n[3], k[3], nk[3], e[3];

	tri_normal(b, c, a, n);
	tri_normal(c, b, d, k);
	x = vec_dot(n, k);
	vec_cross(n, k, nk);
	y = vec_abs(nk);
	ang = atan2(y, x);
	vec_minus(c, b, e);
	if (vec_dot(e, nk) < 0)
		ang = -ang;
	return ang;
}

real
dih_cos(const real a[3], const real b[3], const real c[3], const real d[3])
{

	real n[3], k[3], cos0;

	tri_normal(b, c, a, n);
	tri_normal(c, b, d, k);
	cos0 = -vec_dot(n, k);

	return cos0;
}

real
dih_cos_sup(const real a[3], const real b[3], const real c[3],
			const real d[3])
{
	real n[3], k[3], cos0;

	tri_normal(b, c, a, n);
	tri_normal(c, b, d, k);

	cos0 = vec_dot(n, k);

	return cos0;
}
