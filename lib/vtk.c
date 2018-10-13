#include <stdio.h>
#include <string.h>

#include "real.h"
#include "inc/def.h"
#include "he/err.h"
#include "he/he.h"

#define FMT_IN   XE_REAL_IN
#define FMT_OUT  "%.16g"

#define SIZE (MAX_STRING_SIZE)

int he_vtk_fwrite(He *he, const real *x, const real *y, const real *z, real *scalars[], const char *names[], /**/
                  FILE *f) {
    return HE_OK;
}
