#include <stdio.h>
#include <tgmath.h>

#include "real.h"

#include "co/err.h"
#include "co/matrix.h"
#include "co/memory.h"
#include "co/tri.h"
#include "co/vec.h"
#include "co/edg.h"
#include "co/ring.h"
#include "inc/def.h"

/* #include <alg/pinv.h> */
typedef struct AlgPinv AlgPinv;
int alg_pinv_ini(int dim, AlgPinv**);
int alg_pinv_fin(AlgPinv*);
int alg_pinv_apply(AlgPinv*, real*, /**/ real*);
/**/

#define T Ring
#define N (RANK_MAX + 1)
#define D (6)

static const real pi = 3.141592653589793115997964;

struct T {
    real alpha[N], beta[N], theta[N];
    real xyz[3*N], A[D*N], B[D*D], Binv[D*D], C[D*N];
    AlgPinv *pinv;
};

int ring_ini(T **pq) {
    T *q;
    MALLOC(1, &q);
    alg_pinv_ini(D, &q->pinv);
    *pq = q;
    return CO_OK;
}

int ring_fin(T *q) {
    alg_pinv_fin(q->pinv);
    FREE(q);
    return CO_OK;
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
        ang[s] = 2*pi*alpha[s]/A;

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

int ring_xyz(T *q, int i, const int *ring, const real *x, const real *y, const real *z, /**/ real **pxyz) {
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

static int B(real u, real v, /**/ real **pa) {
    real *a;
    a = *pa;

    *a++ = 1;
    *a++ = u;
    *a++ = v;
    *a++ = u*u/2;
    *a++ = u*v;
    *a++ = v*v/2;

    *pa = a;
    return CO_OK;
}
int ring_A(T *q, int i, const int *ring, const real *x, const real *y, const real *z, real **pA) {
    int s, p, j, k;
    real u, v;
    real *A, *theta;
    real a[3], b[3], r;
    A = q->A;
    vec_get(i, x, y, z, a);

    if (ring_theta(q, i, ring, x, y, z, &theta) != CO_OK)
        ERR(CO_INDEX, "ring_theta failed");

    u = v = 0;
    B(u, v, &A);
    for (s = 0; ring[s] != -1; s++) {
        j = ring[s];
        vec_get(j, x, y, z, /**/ b);
        r = edg_abs(a, b);
        u = r*cos(theta[s]);
        v = r*sin(theta[s]);
        B(u, v, &A);
    }
    *pA = q->A;
    return CO_OK;
}

static int compute_B(int n, const real *A, real *B) {
    return matrix_mult_tn(D, n + 1, D, A, A, B);
}
int ring_B(T *q, int v, const int *ring, const real *x, const real *y, const real *z, real **p) {
    real *A, *B;
    int n;
    B = q->B;
    if (ring_A(q, v, ring, x, y, z, &A) != CO_OK)
        ERR(CO_INDEX, "ring_A failed");
    for (n = 0; ring[n] != -1; n++) ;
    compute_B(n, A, B);
    *p = q->B;
    return CO_OK;
}

static int compute_C(int n, const real *Binv, const real *A, real *C) {
    return matrix_mult_nt(D, D, n + 1,   Binv, A, C);
}
int ring_C(T *q, int v, const int *ring, const real *x, const real *y, const real *z, real **p) {
    real *A, *B, *Binv, *C;
    int n;
    AlgPinv *pinv;
    
    B = q->B;
    C = q->C;
    Binv = q->Binv;
    pinv = q->pinv;

    if (ring_A(q, v, ring, x, y, z, &A) != CO_OK)
        ERR(CO_INDEX, "ring_A failed");
    for (n = 0; ring[n] != -1; n++) ;
    compute_B(n, A, B);
    alg_pinv_apply(pinv, B, /**/ Binv);
    compute_C(n, Binv, A, /**/ C);
    
    *p = q->C;
    return CO_OK;
}
