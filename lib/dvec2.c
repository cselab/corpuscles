#include <stdio.h>

#include "real.h"
#include "co/err.h"
#include "co/dvec2.h"

enum {
	X, Y
};

int
dvec2_cross(const real a[2], const real b[2], real da[2], real db[2])
{
	da[X] = b[Y];
	da[Y] = -b[X];

	db[X] = -a[Y];
	db[Y] = a[X];
	return CO_OK;
}
