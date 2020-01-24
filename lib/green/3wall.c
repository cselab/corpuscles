#include <math.h>
#include <stdio.h>
#ifdef _OPENMP
#include <omp.h>
#endif
#include "real.h"
#include "co/err.h"
#include "co/green/3wall.h"
#include "co/he.h"
#include "co/i/vec.h"
#include "co/i/tri.h"
#include "co/i/area.h"
#include "co/i/matrix.h"
#include "co/macro.h"
#include "co/memory.h"
#include "co/normal.h"
#include "co/ten.h"
#include "co/tensor3x3.h"

#define T Green3Wall
enum { XX, XY, XZ, YX, YY, YZ, ZX, ZY, ZZ };
static const real pi = 3.141592653589793115997964;
struct T {
    real w;
    real *nx, *ny, *nz, *area;
};

static int s0(T *, const real[3], Ten *);

int
green3_wall_ini(He * he, real w, T ** pq)
{
    T *q;
    real *nx, *ny, *nz, *area;
    int n;

    MALLOC(1, &q);
    n = he_nv(he);
    MALLOC3(n, &nx, &ny, &nz);
    MALLOC(n, &area);
    q->w = w;
    q->nx = nx;
    q->ny = ny;
    q->nz = nz;
    q->area = area;
    *pq = q;
    return CO_OK;
}

int
green3_wall_fin(T * q)
{
    FREE3(q->nx, q->ny, q->nz);
    FREE(q->area);
    FREE(q);
    return CO_OK;
}

#define SET(i, j, s, K) i_matrix_set(n, n, i, j, s, t->d[K])
int
green3_wall_apply(T * q, He * he, const real * x, const real * y,
                  const real * z, struct Tensor3x3 *t)
{
    int n;
    int i;
    int k;
    real s;
    Ten t0;

    USED(q);

    n = he_nv(he);
    tensor3x3_zero(n, t);
#pragma omp parallel for
    for (i = 0; i < n; i++) {
        real a[3], b[3];
        int j;

        i_vec_get(i, x, y, z, a);
        for (j = 0; j < n; j++) {
            if (i == j)
                s0(q, a, &t0);
            else {
                i_vec_get(j, x, y, z, b);
                if (green3_wall_s(q, a, b, &t0) != CO_OK)
                    ERR(CO_NUM, "green3_wall_s failed (i=%d, j=%d)", i, j);
            }
            for (k = 0; k < 3 * 3; k++)
                i_matrix_set(n, n, i, j, t0.t[k], t->d[k]);
        }
    }
    s = 1 / (8 * pi);
    tensor3x3_scale(n, s, t);
    return CO_OK;
}

static real
rad(real x, real y, real z)
{
    real r;
    real eps;

    eps = 1e-32;
    r = x * x + y * y + z * z;
    if (r < eps)
        ERR(CO_NUM, "r < eps");
    return sqrt(r);
}

static int
s0(T * q, const real a[3], Ten * t0)
{
    enum {
        X, Y, Z
    };
    real w;
    real *t;
    real r1;
    real r13;
    real r15;
    real w2;
    real zw;

    t = t0->t;
    w = q->w - a[Z];
    zw = -2 * w;
    r1 = 1 / fabs(zw);
    r13 = r1 * r1 * r1;
    r15 = r1 * r1 * r1 * r1 * r1;
    w2 = w * w;
    t[XX] = (-2 * r13 * w2) - r1;
    t[YY] = t[XX];
    t[ZZ] = (-2 * w2 * (12 * r15 * w2 - r13)) - 4 * r13 * w2 - r1;
    t[XY] = 0;
    t[XZ] = 0;
    t[ZX] = 0;
    t[YX] = 0;
    t[YZ] = 0;
    t[ZY] = 0;
    return CO_OK;
}

int
green3_wall_s(T * q, const real a[3], const real b[3], Ten * t0)
{
    enum {
        X, Y, Z
    };
    real d[3];
    real w;
    real *t;
    real r0;
    real r1;
    real x;
    real y;
    real z;
    real r13;
    real r15;
    real r03;
    real w2;
    real x2;
    real y2;
    real z2;
    real zw;
    real zw2;

    t = t0->t;
    w = q->w - b[Z];
    i_vec_minus(a, b, d);

    x = d[X];
    y = d[Y];
    z = d[Z];
    zw = z - 2 * w;
    r0 = 1 / rad(x, y, z);
    r1 = 1 / rad(x, y, zw);
    r13 = r1 * r1 * r1;
    r15 = r1 * r1 * r1 * r1 * r1;
    r03 = r0 * r0 * r0;
    w2 = w * w;
    x2 = x * x;
    y2 = y * y;
    z2 = z * z;
    zw2 = zw * zw;

    t[XX] =
        2 * w * (r13 - 3 * r15 * x2) * zw + 2 * w2 * (r13 - 3 * r15 * x2) -
        r13 * x2 + r03 * x2 - r1 + r0;
    t[YY] =
        2 * w * (r13 - 3 * r15 * y2) * zw + 2 * w2 * (r13 - 3 * r15 * y2) -
        r13 * y2 + r03 * y2 - r1 + r0;
    t[ZZ] =
        2 * w * zw * (3 * r15 * zw2 - r13) + 2 * w2 * (3 * r15 * zw2 -
                                                       r13) - r13 * zw2 +
        r03 * z2 - r1 + r0;
    t[XY] =
        (-6 * r15 * w * x * y * zw) - 6 * r15 * w2 * x * y - r13 * x * y +
        r03 * x * y;
    t[XZ] =
        2 * w * (3 * r15 * x * zw2 - r13 * x) + 6 * r15 * w2 * x * zw -
        r13 * x * zw + r03 * x * z;
    t[ZX] =
        2 * w * ((-3 * r15 * x * zw2) - r13 * x) - 6 * r15 * w2 * x * zw -
        r13 * x * zw + r03 * x * z;
    t[YX] = t[XY];
    t[YZ] =
        2 * w * (3 * r15 * y * zw2 - r13 * y) + 6 * r15 * w2 * y * zw -
        r13 * y * zw + r03 * y * z;
    t[ZY] =
        2 * w * ((-3 * r15 * y * zw2) - r13 * y) - 6 * r15 * w2 * y * zw -
        r13 * y * zw + r03 * y * z;
    return CO_OK;
}

int
green3_wall_t(T * q, const real a[3], const real n[3], const real b[3], Ten * t0)
{
    enum {
        X, Y, Z
    };
    real d[3], r, p, l;

    USED(q);
    i_vec_minus(a, b, d);
    r = i_vec_abs(d);
    p = i_vec_dot(d, n);
    if (r == 0)
        ERR(CO_NUM, "r == 0");
    l = p / (r * r * r * r * r);

    /*
    *xx = d[X] * d[X] * l;
    *xy = d[X] * d[Y] * l;
    *xz = d[X] * d[Z] * l;
    *yy = d[Y] * d[Y] * l;
    *yz = d[Y] * d[Z] * l;
    *zz = d[Z] * d[Z] * l; */
    return CO_OK;
}
