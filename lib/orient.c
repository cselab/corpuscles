#include <stdio.h>

#include "real.h"

#include "he/memory.h"
#include "he/err.h"
#include "he/he.h"
#include "he/vec.h"
#include "he/sum.h"

#include "he/orient.h"

#define T Orient

/* TODO: from alg */
int alg_eig_vectors(const real data[6], /**/ real e0[9]);

struct T { int n; };
static real sum(int n, real *a) { return he_sum_array(n, a); }
int orient_ini(He *he, T **pq) {
    T *q;
    MALLOC(1, &q);
    q->n = he_nv(he);
    *pq = q;
    return HE_OK;
}

int orient_fin(T *q) {
    FREE(q);
    return HE_OK;
}

static int mult(const real m[3*3], real *px, real *py, real *pz) {
    enum {XX, XY, XZ,   YX, YY, YZ,   ZX, ZY, ZZ};
    real x, y, z, x0, y0, z0;
    x = *px; y = *py; z = *pz;

    x0 = x*m[XX] + y*m[YX] + z*m[ZX];
    y0 = x*m[XY] + y*m[YY] + z*m[ZY];
    z0 = x*m[XZ] + y*m[YZ] + z*m[ZZ];

    *px = x0; *py = y0; *pz = z0;
}

static int moment(int n, const real *xx, const real *yy, const real *zz, /**/ real m[6]) {
    /* moment of inertia tensor */
    int i;
    real x, y, z;
    enum {XX, XY, XZ, YY, YZ, ZZ};
    m[XX] = m[XY] = m[XZ] = m[YY] = m[YZ] = m[ZZ] = 0;
    for (i = 0; i < n; i++) {
        x = xx[i]; y = yy[i]; z = zz[i];
        m[XX] += y*y + z*z; m[XY] += -x*y; m[XZ] += -x*z;
        m[YY] += x*x + z*z; m[YZ] += -y*z;
        m[ZZ] += x*x + y*y;
    }
    return HE_OK;
}

static int to_cm(int n, /**/ real *xx, real *yy, real *zz) {
    int i;
    real x, y, z;
    x = sum(n, xx)/n; y = sum(n, yy)/n; z = sum(n, zz)/n;
    for (i = 0; i < n; i++) {
        xx[i] -= x; yy[i] -= y; zz[i] -= z;
    }
}

int orient_apply(T *q, /**/ real *x, real *y, real *z) {
    int n, i, j;
    real x0, y0, z0;
    real m[6], v[3*3];
    n = q->n;
    to_cm(n, /**/ x, y, z);
    moment(n, x, y, z, /**/ m);
    alg_eig_vectors(m, v);
    for (i = 0; i < n; i++)
        mult(v, /**/ &x[i], &y[i], &z[i]);
    return HE_OK;
}
