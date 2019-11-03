#include <stdio.h>
#include <tgmath.h>
#include "real.h"
#include "co/err.h"
#include "co/array.h"

#define OUT CO_REAL_OUT

real
array_min(int n, const real a[])
{
    int i;
    real x;

    for (i = 0, x = a[0]; i < n; i++)
        if (a[i] < x)
            x = a[i];
    return x;
}

real
array_min3(int n, const real a[], const real b[], const real c[])
{
    real A, B, C;

    A = array_min(n, a);
    B = array_min(n, b);
    C = array_min(n, c);

    /* A < B < C */
    if (C < B)
        B = C;
    if (B < A)
        A = B;
    return A;
}


real
array_max(int n, const real a[])
{
    int i;
    real x;

    for (i = 0, x = a[0]; i < n; i++)
        if (a[i] > x)
            x = a[i];
    return x;
}

real
array_max3(int n, const real a[], const real b[], const real c[])
{
    real A, B, C;

    A = array_max(n, a);
    B = array_max(n, b);
    C = array_max(n, c);

    /* A > B > C */
    if (C > B)
        B = C;
    if (B > A)
        A = B;
    return A;
}

int
array_zero(int n, real a[])
{
    int i;

    if (n < 0)
        ERR(CO_INDEX, "n=%d < 0", n);
    for (i = 0; i < n; i++)
        a[i] = 0;
    return CO_OK;
}

int
array_zero3(int n, real a[], real b[], real c[])
{
    if (n < 0)
        ERR(CO_INDEX, "n=%d < 0", n);
    array_zero(n, a);
    array_zero(n, b);
    array_zero(n, c);
    return CO_OK;
}

int
array_one(int n, real a[])
{
    int i;

    if (n < 0)
        ERR(CO_INDEX, "n=%d < 0", n);
    for (i = 0; i < n; i++)
        a[i] = 1;
    return CO_OK;
}

int
array_axpy(int n, real a, const real x[], /**/ real y[])
{
    int i;

    for (i = 0; i < n; i++)
        y[i] += a * x[i];
    return CO_OK;
}

int
array_axpy3(int n, real a,
            const real x0[], const real x1[], const real x2[],
            real y0[], real y1[], real y2[])
{
    array_axpy(n, a, x0, y0);
    array_axpy(n, a, x1, y1);
    array_axpy(n, a, x2, y2);
    return CO_OK;
}

int
array_copy(int n, const real * a, real * b)
{
    int i;

    for (i = 0; i < n; i++)
        b[i] = a[i];
    return CO_OK;
}

int
array_copy3(int n, const real * x, const real * y, const real * z,
            real * u, real * v, real * w)
{
    array_copy(n, x, u);
    array_copy(n, y, v);
    array_copy(n, z, w);
    return CO_OK;
}

int
array_negative(int n, const real * a, real * b)
{
    int i;

    for (i = 0; i < n; i++)
        b[i] = -a[i];
    return CO_OK;
}

int
array_neg(int n, real * a)
{
    int i;

    for (i = 0; i < n; i++)
        a[i] = -a[i];
    return CO_OK;
}

real
array_dot(int n, const real * a, const real * b)
{
    int i;
    real s;

    s = 0;
    for (i = 0; i < n; i++)
        s += a[i] * b[i];
    return s;
}

real
array_mean(int n, const real * a)
{
    int i;
    real s;

    s = 0;
    for (i = 0; i < n; i++)
        s += a[i];
    return s / n;
}

real
array_msq(int n, const real * a)
{
    int i;
    real s;

    s = 0;
    for (i = 0; i < n; i++)
        s += a[i] * a[i];
    return sqrt(s / n);
}

real
array_msq_3d(int n, const real * a, const real * b, const real * c)
{
    int i;
    real s;

    s = 0;
    for (i = 0; i < n; i++)
        s += a[i] * a[i] + b[i] * b[i] + c[i] * c[i];
    return sqrt(s / n / 3.0);
}

real
array_l2(int n, const real * a, const real * b)
{
    int i;
    real s, d;

    s = 0;
    for (i = 0; i < n; i++) {
        d = a[i] - b[i];
        s += d * d;
    }
    return sqrt(s / n);
}

real
array_l2_3d(int n, const real * a1, const real * a2, const real * b1,
            const real * b2, const real * c1, const real * c2)
{
    int i;
    real s, d;

    s = 0;
    for (i = 0; i < n; i++) {
        d = a1[i] - a2[i];
        s += d * d;
        d = b1[i] - b2[i];
        s += d * d;
        d = c1[i] - c2[i];
        s += d * d;
    }
    return sqrt(s / n / 3.0);
}

real
array_norm(int n, const real * a)
{
    int i;
    real s;

    s = 0;
    for (i = 0; i < n; i++)
        s += a[i] * a[i];
    return sqrt(s / n);
}

real
array_diff(int n, const real * a, const real * b)
{
    int i;
    real s, d, p;

    s = 0;
    for (i = 0; i < n; i++) {
        d = a[i] - b[i];
        s += d * d;
    }
    s = sqrt(s / n);
    p = array_norm(n, a);
    return p == 0 ? s : s / p;
}


real
array_norm3(int n, const real * x, const real * y, const real * z)
{
    int i;
    real s;

    s = 0;
    for (i = 0; i < n; i++)
        s += x[i] * x[i] + y[i] * y[i] + z[i] * z[i];
    return sqrt(s / n);
}

static real
sq(real x)
{
    return x * x;
}

real
array_diff3(int n, const real * x, const real * y, const real * z,
            const real * u, const real * v, const real * w)
{
    int i;
    real s, p;

    s = 0;
    for (i = 0; i < n; i++) {
        s += sq(x[i] - u[i]);
        s += sq(y[i] - v[i]);
        s += sq(z[i] - w[i]);
    }
    s = sqrt(s / n);
    p = array_norm3(n, x, y, z);
    return p == 0 ? s : s / p;
}

int
array_scale(int n, real s, real * a)
{
    int i;

    for (i = 0; i < n; i++)
        a[i] *= s;
    return CO_OK;
}

int
array_scale3(int n, real s, real * x, real * y, real * z)
{
    array_scale(n, s, x);
    array_scale(n, s, y);
    array_scale(n, s, z);
    return CO_OK;
}

int
array_multiply(int n, const real * a, real * x)
{
    int i;

    for (i = 0; i < n; i++)
        x[i] *= a[i];
    return CO_OK;
}

int
array_multiply3(int n, const real * a, real * x, real * y, real * z)
{
    int i;

    for (i = 0; i < n; i++) {
        x[i] *= a[i];
        y[i] *= a[i];
        z[i] *= a[i];
    }
    return CO_OK;
}

int
array_fwrite(FILE * f, int n, const real * a)
{
    int i, status;

    for (i = 0; i < n; i++) {
        status = fprintf(f, OUT "\n", a[i]);
        if (status < 0)
            ERR(CO_IO, "fprintf failed");
    }
    return CO_OK;
}
