#include <stdio.h>

#include "real.h"
#include "co/err.h"
#include "co/vec2.h"
#include "co/dtri2.h"

int
dtri2_angle(const real a[2], const real b[2], const real c[2], real da[2], real db[2], real dc[2])
{
	real u[2], v[2];

	vec2_minus(a, b, u);
	vec2_minus(c, b, v);
	vec2_cov(u, da);
	vec2_cov(v, dc);
	vec2_neg(dc);
	vec2_plus(da, dc, db);
	vec2_neg(db);
	return CO_OK;
}