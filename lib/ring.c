#include <stdio.h>

#include "real.h"

#include "co/err.h"
#include "co/memory.h"
#include "co/tri.h"
#include "co/vec.h"
#include "co/ring.h"
#include "inc/def.h"

#define T Ring
#define N (RANK_MAX + 1)

struct T {
    real alpha[N], beta[N], theta[N];
    real xyz[3*N], A[6*N];
};

int ring_ini(T **pq) {
    T *q;
    MALLOC(1, &q);
    *pq = q;
    return CO_OK;
}

int ring_fin(T *q) {
    FREE(q);
}

static int get2(int i, int j,
                const real *x, const real *y, const real *z,
                real a[3], real b[3]) {
    vec_get(i, x, y, z, a);
    vec_get(j, x, y, z, b);
    return CO_OK;
}
int ring_alpha(T *q, int i, const int *ring, const real *x, const real *y, const real *z, real **pang) {
    int s, p, j, k;
    real *ang;
    real a[3], b[3], c[3];
    ang = q->alpha;
    vec_get(i, x, y, z, a);
    for (s = 0; ring[s] != -1; s++) {
        j = ring[s];
        k = ring[s + 1];
        if (k == -1)
            k = ring[0];
        get2(j, k, x, y, z, /**/ b, c);
        ang[s] = tri_angle(c, a, b);
    }
    *pang = ang;
    return CO_OK;
}

int ring_beta(T *q, int i, const int *ring, const real *x, const real *y, const real *z, real **pang) {
    int s, p, j, k;
    real *ang, *alpha, A;

    ring_alpha(q, i, ring, x, y, z, /**/ &alpha);
    ang = q->beta;
    A = 0;
    for (s = 0; ring[s] != -1; s++)
        A += alpha[s];

    if (A == 0)
        ERR(CO_NUM, "A == 0");

    for (s = 0; ring[s] != -1; s++)
        ang[s] = alpha[s] / A;
    
    *pang = ang;
    return CO_OK;
}

int ring_theta(T *q, int i, const int *ring, const real *x, const real *y, const real *z, real **pang) {
    int s, p, j, k;
    real *ang, *beta, A;

    ring_beta(q, i, ring, x, y, z, /**/ &beta);
    ang = q->theta;

    ang[0] = 0;
    for (s = 1; ring[s] != -1; s++)
        ang[s] = ang[s - 1] + beta[s];

    *pang = ang;
    return CO_OK;
}

int ring_xyz(T *q, int i, const int *ring, const real *x, const real *y, const real *z, real **pxyz) {
    int s, p, j, k;
    real *xyz;
    real a[3];
    xyz = q->xyz;
    k = 0;
    xyz[k++] = x[i]; xyz[k++] = y[i]; xyz[k++] = z[i];
    for (s = 0; ring[s] != -1; s++) {
        i = ring[s];
        xyz[k++] = x[i]; xyz[k++] = y[i]; xyz[k++] = z[i];
    }
    *pxyz = xyz;
    return CO_OK;
}
