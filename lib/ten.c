#include "real.h"
#include "he/err.h"
#include "he/ten.h"

enum {X, Y, Z};
enum {XX, XY, XZ,   YX, YY, YZ,   ZX, ZY, ZZ};
void ten_dyadic(real a[3], real b[3], /**/ Ten T) {
    real *t;
    t = T.t;
    t[XX] = a[X]*b[X]; t[XY] = a[X]*b[Y]; t[XZ] = a[X]*b[Z];
    t[YX] = a[Y]*b[X]; t[YY] = a[Y]*b[Y]; t[YZ] = a[Y]*b[Z];
    t[ZX] = a[Z]*b[X]; t[ZY] = a[Z]*b[Y]; t[ZZ] = a[Z]*b[Z];
}

