#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>

#include <real.h>
#include <he/err.h>
#include <he/vec.h>
#include <he/tri.h>
#include <he/macro.h>
#include <he/util.h>

int main(void) {
    real a[3], b[3], c[3], A;
    A = tri_area(a, b, c);
    printf(HE_REAL_OUT "\n", A);
}
