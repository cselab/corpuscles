#include "real.h"
#include "he/err.h"

real array_min(int n, const real a[]) {
    int i;
    real x;
    for (i = 0, x = a[0]; i < n; i++)
        if (a[i] < x) x = a[i];
    return x;
}

real array_max(int n, const real a[]) {
    int i;
    real x;
    for (i = 0, x = a[0]; i < n; i++)
        if (a[i] > x) x = a[i];
    return x;
}

int array_zero(int n, real a[]) {
    int i;
    if (n < 0)
        ERR(HE_INDEX, "n=%d < 0", n);
    for (i = 0; i < n; i++)
        a[i] = 0;
    return HE_OK;
}
