#include "real.h"
#include "co/err.h"
#include "co/array.h"

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
        ERR(CO_INDEX, "n=%d < 0", n);
    for (i = 0; i < n; i++)
        a[i] = 0;
    return CO_OK;
}

int array_zero3(int n, real a[], real b[], real c[]) {
    if (n < 0)
        ERR(CO_INDEX, "n=%d < 0", n);
    array_zero(n, a);
    array_zero(n, b);
    array_zero(n, c);
    return CO_OK;
}

int array_axpy(int n, real a, const real x[], /**/ real y[]) {
    int i;
    for (i = 0; i < n; i++)
        y[i] += a * x[i];
    return CO_OK;
}

int array_axpy3(int n, real a,
                const real x0[], const real x1[], const real x2[],
                real y0[], real y1[], real y2[]) {
    array_axpy(n, a, x0, y0);
    array_axpy(n, a, x1, y1);
    array_axpy(n, a, x2, y2);
    return CO_OK;
}

int array_copy(int n, const real* a, real *b) {
    int i;
    for (i = 0; i < n; i++)
        b[i] = a[i];
    return CO_OK;
}

real array_dot(int n, const real *a, const real *b) {
    int i;
    real s;
    s = 0;
    for (i = 0; i < n; i++)
        s += a[i] * b[i];
    return s;
}

