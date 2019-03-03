#include <stdio.h>

#include <real.h>
#include <co/tri.h>

#define FMT CO_REAL_OUT

int main(void) {
    enum {X, Y, Z};
    real a[3], b[3], c[3], A;

    a[X] = 0; a[Y] = 0; a[Z] = 0;
    b[X] = 1; b[Y] = 0; b[Z] = 0;
    c[X] = 0; c[Y] = 1; c[Z] = 0;

    A = tri_area(a, b, c);
    printf("Area is " FMT "\n", A);
}
