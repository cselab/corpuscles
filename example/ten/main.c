#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <real.h>
#include <he/err.h>
#include <he/vec.h>
#include <he/ten.h>

int main() {
    Ten T, R, P;
    ten_one(&T);
    ten_one(&R);
    ten_plus(T, T, /**/ &P);
    ten_printf(P, "%-6.2f");
    return 0;
}
