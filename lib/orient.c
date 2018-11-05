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

static int transpose(/*io*/ real v[9]) {
    enum {XX, XY, XZ,
          YX, YY, YZ,
          ZX, ZY, ZZ};
    real t;
    t = v[XY]; v[XY] = v[YX]; v[YX] = t;
    t = v[XZ]; v[XZ] = v[ZX]; v[ZX] = t;
    t = v[YZ]; v[YZ] = v[ZY]; v[ZY] = t;
    return HE_OK;
}

int orient_apply(T *q, /**/ real *xx, real *yy, real *zz) {
    enum {XX, XY, XZ, YY, YZ, ZZ};
    int n, i, j;
    real x0, y0, z0, x, y, z;
    real m[6], v[3*3];

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

    alg_eig_vectors(m, v);
    transpose(v);
    MSG("v[1,0]: %g", v[1 + 3*0]);
    MSG("v[0,1]: %g", v[0 + 3*1]);

    for (i = 0; i < n; i++) {
        x = xx[i]; y = yy[i]; z = zz[i];
        j = 0;
        x0 = v[j++]*x + v[j++]*y + v[j++]*z;
        y0 = v[j++]*x + v[j++]*y + v[j++]*z;
        z0 = v[j++]*x + v[j++]*y + v[j++]*z;
        xx[i] = x0; yy[i] = y0; zz[i] = z0;
    }

    return HE_OK;
}
