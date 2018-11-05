#include <stdio.h>

#include "real.h"
#include "he/memory.h"
#include "he/err.h"
#include "he/he.h"
#include "he/vec.h"
#include "he/sum.h"

#include "he/orient.h"

#define T Orient

struct T {
    int n;
    real *x, *y, *z;
};

static real sum(int n, real *a) { return he_sum_array(n, a); }

int orient_ini(He *he, T **pq) {
    T *q;
    int n;
    MALLOC(1, &q);

    n = he_nv(he);

    q->n = n;
    MALLOC(n, &q->x); MALLOC(n, &q->y); MALLOC(n, &q->z);

    *pq = q;
    return HE_OK;
}

int orient_fin(T *q) {
    FREE(q->x); FREE(q->y); FREE(q->z);
    FREE(q);
    return HE_OK;
}

static int inv3x3(const real m[6], /**/ real r[6]) {
    enum {XX, XY, XZ, YY, YZ, ZZ};
    real eps = 1e-12;
    real xx, yy, zz, xy, xz, yz;
    real yz2, xz2, xy2;
    real mx, my, mz; /* minors */
    real d, i; /* determinant and its inverse */

    xx = m[XX]; yy = m[YY]; zz = m[ZZ];
    xy = m[XY]; xz = m[XZ];
    yz = m[YZ];
    yz2 = yz*yz; xz2 = xz*xz; xy2 = xy*xy;

    mx  = yy*zz-yz2;
    my  = xy*zz-xz*yz;
    mz  = xy*yz-xz*yy;
    d   = mz*xz-my*xy+mx*xx;
    if (d < eps && d > -eps)
        ERR(HE_NUM, "Matrix is singular, determinant %.16g", d);
    i   = 1/d;

    r[XX] =  mx*i;
    r[XY] = -my*i;
    r[XZ] =  mz*i;
    r[YY] = i*(xx*zz-xz2);
    r[YZ] = i*(xy*xz-xx*yz);
    r[ZZ] = i*(xx*yy-xy2);
    return HE_OK;
}


int orient_apply(T *q, /**/ real *xx, real *yy, real *zz) {
    enum {XX, XY, XZ, YY, YZ, ZZ};    
    int n, i;
    real x0, y0, z0, x, y, z;
    real m[6], r[6];

    n = q->n;

    x0 = sum(n, xx)/n; y0 = sum(n, yy)/n; z0 = sum(n, zz)/n;
    for (i = 0; i < n; i++) {
        xx[i] -= x0; yy[i] -= y0; zz[i] -= z0;
    }

    m[XX] = m[XY] = m[XZ] = m[YY] = m[YZ] = m[ZZ] = 0;
    for (i = 0; i < n; i++) {
        x = xx[i]; y = yy[i]; z = zz[i];
	m[XX] += y*y + z*z; m[XY] += -x*y; m[XZ] += -x*z;
        m[YY] += x*x + z*z; m[YZ] += -y*z;
        m[ZZ] += x*x + y*y;
    }

    inv3x3(m, r);
    for (i = 0; i < n; i++) {
        x = xx[i]; y = yy[i]; z = zz[i];
        x0 = r[XX]*x + r[XY]*y + r[XZ]*z;
        y0 = r[XY]*x + r[YY]*y + r[YZ]*z;
        z0 = r[XZ]*x + r[YZ]*y + r[ZZ]*z;
        xx[i] = x0; yy[i] = y0; zz[i] = z0;
    }

    return HE_OK;
}
