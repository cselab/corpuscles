#include <stdio.h>

#include "real.h"
#include "he/err.h"

#define SET(key, val) if ((key) != NULL) *(key) = (val)
static real sq(real x) { return x*x; }

#include "he/linear_strain_tri.h"
#include "linear_strain_tri.inc"
