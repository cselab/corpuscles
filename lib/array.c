#include "real.h"

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
