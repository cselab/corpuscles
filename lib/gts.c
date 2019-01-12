#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "real.h"
#include "he/err.h"
#include "he/util.h"
#include "he/he.h"
#include "inc/def.h"

#include "he/gts.h"

#define OUT HE_REAL_OUT

int gts_fwrite(He *he, const real *x, const real *y, const real *z, /**/ FILE *f) {
    return HE_OK;
}
