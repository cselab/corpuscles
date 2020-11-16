#include <stdio.h>
#include <stdlib.h>
#include <gsl/gsl_spline.h>
#include <gsl/gsl_errno.h>

#include <real.h>
#include <co/err.h>
#include <co/memory.h>
#include <co/real.h>

#include "alg/spline.h"

#define T AlgSpline
#define MAXLINE (2048)
#define IN CO_REAL_IN

struct T {
    gsl_spline *s;
    gsl_interp_accel *acc;
    real xl, xh, yl, yh;
};

int
alg_spline_ini(int n, real * x, real * y, int type, T ** pq)
{
    T *q;
    gsl_spline *s;
    gsl_interp_accel *acc;
    Real *xr, *yr;

    double *xd, *yd;

    if (n <= 0)
        ERR(CO_INDEX, "n=%d <= 0", n);
    MALLOC(1, &q);
    switch (type) {
    case LINEAR:
        s = gsl_spline_alloc(gsl_interp_linear, n);
        break;
    case POLYNOMIAL:
        s = gsl_spline_alloc(gsl_interp_polynomial, n);
        break;
    case CSPLINE:
        s = gsl_spline_alloc(gsl_interp_cspline, n);
        break;
    case AKIMA:
        s = gsl_spline_alloc(gsl_interp_akima, n);
        break;
    case STEFFEN:
        s = gsl_spline_alloc(gsl_interp_steffen, n);
        break;
    default:
        ERR(CO_INDEX, "unknown spline type: %d", type);
    }
    if (s == NULL)
        ERR(CO_MEMORY, "fail to allocate spline");

    real_ini(n, &xr);
    real_ini(n, &yr);
    xd = real_to(xr, x);
    yd = real_to(yr, y);
    if (gsl_spline_init(s, xd, yd, n) != GSL_SUCCESS)
        ERR(CO_MEMORY, "fail to init spline");
    real_fin(xr);
    real_fin(yr);

    acc = gsl_interp_accel_alloc();
    if (acc == NULL)
        ERR(CO_MEMORY, "fail to allocate accel");

    q->xl = x[0];
    q->xh = x[n - 1];

    q->yl = y[0];
    q->yh = y[n - 1];

    q->acc = acc;
    q->s = s;
    *pq = q;
    return CO_OK;
}

int
alg_spline_fscanf(FILE * f, int type, T ** pq)
{
    real *x, *y;
    char line[MAXLINE];
    int n, i, N;

    N = 10;
    MALLOC(N, &x);
    MALLOC(N, &y);
    n = 0;
    while (fgets(line, MAXLINE, f) != NULL) {
        if (sscanf(line, IN " " IN, &x[n], &y[n]) != 2)
            ERR(CO_IO, "fail to parse '%s' at line number %d", line,
                n + 1);
        n++;
        if (n >= N) {
            N *= 2;
            REALLOC(N, &x);
            REALLOC(N, &y);
        }
    }
    for (i = 0; i < n - 1; i++)
        if (x[i + 1] <= x[i])
            ERR(CO_IO, "x[%d] <= x[%d]", i, i + 1);
    if (n == 0)
        ERR(CO_IO, "fail to read file");
    if (alg_spline_ini(n, x, y, type, pq) != CO_OK)
        ERR(CO_IO, "ini failed");
    FREE(x);
    FREE(y);
    return CO_OK;
}

int
alg_spline_fin(T * q)
{
    gsl_spline_free(q->s);
    gsl_interp_accel_free(q->acc);
    FREE(q);
    return CO_OK;
}

real
alg_spline_apply(T * q, real x)
{
    if (x >= q->xh)
        return q->yh;
    else if (x <= q->xl)
        return q->yl;
    else
        return gsl_spline_eval(q->s, x, q->acc);
}

real
alg_spline_lo(T * q)
{
    return q->xl;
}

real
alg_spline_hi(T * q)
{
    return q->xh;
}
