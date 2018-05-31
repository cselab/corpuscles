#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <real.h>
#include <he/err.h>
#include <he/vec.h>
#include <he/ten.h>

int main() {
    real a[3] = {1, 2, 3}, b[3] = {10, 20, 30};
    Ten T, R, P;
    ten_dyadic(a, b, &T);
    ten_one(&R);
    ten_plus(T, T, &P);
    ten_printf(P, "%-6.2f");
    return 0;
}
