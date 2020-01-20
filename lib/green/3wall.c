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
#include "co/tensor3x3.h"

#define T Green3Wall
enum {XX, XY, XZ, YX, YY, YZ, ZX, ZY, ZZ};
static const real pi = 3.141592653589793115997964;
struct T {
    real w;
    real *nx, *ny, *nz, *area;
};

int
green3_wall_ini(He * he, T ** pq)
{
    T *q;
    real *nx, *ny, *nz, *area;
    int n;

    MALLOC(1, &q);
    n = he_nv(he);
    MALLOC3(n, &nx, &ny, &nz);
    MALLOC(n, &area);
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
    int n, i;
    real s;

    USED(q);

    n = he_nv(he);
    tensor3x3_zero(n, t);
#pragma omp parallel for
    for (i = 0; i < n; i++) {
        real a[3], b[3], xx, xy, xz, yy, yz, zz;
        int j;
        i_vec_get(i, x, y, z, a);
        for (j = 0; j < n; j++) {
	    if (i == j) continue;
            i_vec_get(j, x, y, z, b);
            if (green3_wall_s(q, a, b, &xx, &xy, &xz, &yy, &yz, &zz) != CO_OK)
                ERR(CO_NUM, "ossen failed (i=%d, j=%d)", i, j);
        }
    }
    s = 1 / (8 * pi);
    tensor3x3_scale(n, s, t);
    return CO_OK;
}

int
green3_wall_s(T * q, const real a[3], const real b[3],
	      real * xx, real * xy, real * xz, real * yy, real * yz, real * zz)
{
    enum {
        X, Y, Z
    };
    real d[3], r, r3, l;
    real *yx, *zy, *zx;
    real w;

    w = q->w;
    i_vec_minus(a, b, d);
    r = i_vec_abs(d);
    r3 = r * r * r;
    l = 1 / r;
    if (r == 0)
        ERR(CO_NUM, "r == 0");
    *xx = l + d[X] * d[X] / r3;
    *yy = l + d[Y] * d[Y] / r3;
    *zz = l + d[Z] * d[Z] / r3;
    *xy = d[X] * d[Y] / r3;
    *xz = d[X] * d[Z] / r3;
    *yz = d[Y] * d[Z] / r3;
    return CO_OK;
}

int
green3_wall_t(T * q, const real a[3], const real n[3], const real b[3],
          real * xx, real * xy, real * xz, real * yy, real * yz, real * zz)
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
    *xx = d[X] * d[X] * l;
    *xy = d[X] * d[Y] * l;
    *xz = d[X] * d[Z] * l;
    *yy = d[Y] * d[Y] * l;
    *yz = d[Y] * d[Z] * l;
    *zz = d[Z] * d[Z] * l;
    return CO_OK;
}
