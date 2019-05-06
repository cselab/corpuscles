#include <stdio.h>

#include "real.h"
#include "co/err.h"
#include "co/vec2.h"

#include "co/dedg2.h"

int
dedg2_abs(const real a[2], const real b[2], real da[2], real db[2])
{
	vec2_minus(b, a,   db);
	vec2_normalize(db);
	vec2_negative(db, da);
	return CO_OK;
}

int
dedg2_sq(const real a[2], const real b[2], real da[2], real db[2])
{
	vec2_minus(b, a,   db);
	vec2_scale(2,  db);
	vec2_negative(db,    da);
	return CO_OK;
}
