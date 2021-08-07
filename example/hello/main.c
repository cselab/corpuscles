#include <stdio.h>

#include <real.h>
#include <co/tri.h>

#define FMT CO_REAL_OUT

int
main(void)
{
    real A, a[] = { 0, 0, 0 }, b[] = { 2, 0, 0 }, c[] = { 0, 1, 0 };
    A = tri_area(a, b, c);
    printf("Area is of triangle: " FMT "\n", A);
}
