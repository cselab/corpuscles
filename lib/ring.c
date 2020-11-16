#include <stdio.h>
#include <math.h>

#include "real.h"

#include "co/array.h"
#include "co/edg.h"
#include "co/err.h"
#include "co/matrix.h"
#include "co/memory.h"
#include "co/ring.h"
#include "co/tri.h"
#include "co/vec.h"
#include "inc/def.h"
#include "alg/pinv.h"

#define T Ring
#define N (RANK_MAX + 1)
#define D (6)                   /* number of basis functions */
static const real pi = 3.141592653589793115997964;

struct T {
    real alpha[N], beta[N], theta[N];
    real scalar[N], xyz[3 * N], A[D * N], B[D * D], Binv[D * D], C[D * N],
        wgrad[3 * N];
    AlgPinv *pinv;
};

int
ring_ini(T ** pq)
{
    T *q;

    MALLOC(1, &q);
    alg_pinv_ini(D, &q->pinv);
    *pq = q;
    return CO_OK;
}

int
ring_fin(T * q)
{
    alg_pinv_fin(q->pinv);
    FREE(q);
    return CO_OK;
}

static int
get2(int i, int j,
     const real * x, const real * y, const real * z, real a[3], real b[3])
{
    vec_get(i, x, y, z, a);
    vec_get(j, x, y, z, b);
    return CO_OK;
}

int
ring_alpha(T * q, int i, const int *ring, const real * x, const real * y,
           const real * z, real ** pang)
{
    int s, j, k;
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

int
ring_beta(T * q, int i, const int *ring, const real * x, const real * y,
          const real * z, real ** pang)
{
    int s;
    real *ang, *alpha, A;

    ring_alpha(q, i, ring, x, y, z, /**/ &alpha);
    ang = q->beta;
    A = 0;
    for (s = 0; ring[s] != -1; s++)
        A += alpha[s];

    if (A == 0)
        ERR(CO_NUM, "A == 0");

    for (s = 0; ring[s] != -1; s++)
        ang[s] = 2 * pi * alpha[s] / A;

    *pang = ang;
    return CO_OK;
}

int
ring_theta(T * q, int i, const int *ring, const real * x, const real * y,
           const real * z, real ** pang)
{
    int s;
    real *ang, *beta;

    ring_beta(q, i, ring, x, y, z, /**/ &beta);
    ang = q->theta;

    ang[0] = 0;
    for (s = 1; ring[s] != -1; s++)
        ang[s] = ang[s - 1] + beta[s];

    *pang = ang;
    return CO_OK;
}

int
ring_scalar(T * q, int i, const int *ring, const real * s,
            /**/ real ** pscalar)
{
    int n, k;
    real *scalar;

    scalar = q->scalar;
    k = 0;
    scalar[k++] = s[i];
    for (n = 0; ring[n] != -1; n++) {
        i = ring[n];
        scalar[k++] = s[i];
    }
    *pscalar = scalar;
    return CO_OK;
}

int
ring_xyz(T * q, int i, const int *ring, const real * x, const real * y,
         const real * z, /**/ real ** pxyz)
{
    int n, k;
    real *xyz;

    xyz = q->xyz;
    k = 0;
    xyz[k++] = x[i];
    xyz[k++] = y[i];
    xyz[k++] = z[i];
    for (n = 0; ring[n] != -1; n++) {
        i = ring[n];
        xyz[k++] = x[i];
        xyz[k++] = y[i];
        xyz[k++] = z[i];
    }
    matrix_transpose(n + 1, 3, xyz);

    *pxyz = xyz;
    return CO_OK;
}

static int
B(real u, real v, /**/ real ** pa)
{
    real *a;

    a = *pa;

    *a++ = 1;
    *a++ = u;
    *a++ = v;
    *a++ = u * u / 2;
    *a++ = u * v;
    *a++ = v * v / 2;

    *pa = a;
    return CO_OK;
}

int
ring_A(T * q, int i, const int *ring, const real * x, const real * y,
       const real * z, real ** pA)
{
    int s, j;
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
        u = r * cos(theta[s]);
        v = r * sin(theta[s]);
        B(u, v, &A);
    }
    *pA = q->A;
    return CO_OK;
}

static int
compute_B(int n, const real * A, real * B)
{
    return matrix_mult_tn(D, n + 1, D, A, A, B);
}

int
ring_B(T * q, int v, const int *ring, const real * x, const real * y,
       const real * z, real ** p)
{
    real *A, *B;
    int n;

    B = q->B;
    if (ring_A(q, v, ring, x, y, z, &A) != CO_OK)
        ERR(CO_INDEX, "ring_A failed");
    for (n = 0; ring[n] != -1; n++);
    compute_B(n, A, B);
    *p = q->B;
    return CO_OK;
}

static int
compute_C(int n, const real * Binv, const real * A, real * C)
{
    return matrix_mult_nt(D, D, n + 1, Binv, A, C);
}

int
ring_C(T * q, int v, const int *ring, const real * x, const real * y,
       const real * z, real ** p)
{
    real *A, *B, *Binv, *C;
    int n;
    AlgPinv *pinv;

    B = q->B;
    C = q->C;
    Binv = q->Binv;
    pinv = q->pinv;

    if (ring_A(q, v, ring, x, y, z, &A) != CO_OK)
        ERR(CO_INDEX, "ring_A failed");
    for (n = 0; ring[n] != -1; n++);
    compute_B(n, A, B);
    alg_pinv_apply(pinv, B, /**/ Binv);
    compute_C(n, Binv, A, /**/ C);

    *p = q->C;
    return CO_OK;
}

int
ring_xu(int n, const real * xyz, const real * C, /**/ real x[3])
{
    const real *C1;

    C1 = C + (n + 1);
    matrix_mult_nt(3, n + 1, 1, xyz, C1, x);
    return CO_OK;
}

int
ring_xv(int n, const real * xyz, const real * C, /**/ real x[3])
{
    const real *C1;

    C1 = C + 2 * (n + 1);
    matrix_mult_nt(3, n + 1, 1, xyz, C1, x);
    return CO_OK;
}

int
ring_xuu(int n, const real * xyz, const real * C, /**/ real x[3])
{
    const real *C1;

    C1 = C + 3 * (n + 1);
    matrix_mult_nt(3, n + 1, 1, xyz, C1, x);
    return CO_OK;
}

int
ring_xuv(int n, const real * xyz, const real * C, /**/ real x[3])
{
    const real *C1;

    C1 = C + 4 * (n + 1);
    matrix_mult_nt(3, n + 1, 1, xyz, C1, x);
    return CO_OK;
}

int
ring_xvv(int n, const real * xyz, const real * C, /**/ real x[3])
{
    const real *C1;

    C1 = C + 5 * (n + 1);
    matrix_mult_nt(3, n + 1, 1, xyz, C1, x);
    return CO_OK;
}

real
ring_guu(int n, const real * xyz, const real * C)
{
    real x[3];

    ring_xu(n, xyz, C, /**/ x);
    return vec_dot(x, x);
}

real
ring_guv(int n, const real * xyz, const real * C)
{
    real u[3], v[3];

    ring_xu(n, xyz, C, /**/ u);
    ring_xv(n, xyz, C, /**/ v);
    return vec_dot(u, v);
}

real
ring_gvv(int n, const real * xyz, const real * C)
{
    real x[3];

    ring_xv(n, xyz, C, /**/ x);
    return vec_dot(x, x);
}

int
ring_gcov(int n, const real * xyz, const real * C, /**/ real g[3])
{
    enum { UU, UV, VV };
    real xu[3], xv[3];

    ring_xu(n, xyz, C, /**/ xu);
    ring_xv(n, xyz, C, /**/ xv);

    g[UU] = vec_dot(xu, xu);
    g[UV] = vec_dot(xu, xv);
    g[VV] = vec_dot(xv, xv);
    return CO_OK;
}

real
ring_g(int n, const real * xyz, const real * C)
{
    enum { UU, UV, VV };
    real g[3];

    ring_gcov(n, xyz, C, /**/ g);
    return g[UU] * g[VV] - g[UV] * g[UV];
}

int
ring_gcnt(int n, const real * xyz, const real * C, /**/ real ginv[3])
{
    enum { UU, UV, VV };
    real g[3], g0;

    ring_gcov(n, xyz, C, /**/ g);
    g0 = g[UU] * g[VV] - g[UV] * g[UV];
    if (g0 == 0)
        ERR(CO_NUM, "g0 = 0");
    ginv[UU] = g[VV] / g0;
    ginv[UV] = -g[UV] / g0;
    ginv[VV] = g[UU] / g0;
    return CO_OK;
}

real
ring_buu(int n, const real * xyz, const real * C)
{
    real x[3], norm[3];;
    ring_xuu(n, xyz, C, /**/ x);
    ring_normal(n, xyz, C, norm);
    return vec_dot(x, norm);
}

real
ring_buv(int n, const real * xyz, const real * C)
{
    real x[3], norm[3];;
    ring_xuv(n, xyz, C, /**/ x);
    ring_normal(n, xyz, C, norm);
    return vec_dot(x, norm);
}

real
ring_bvv(int n, const real * xyz, const real * C)
{
    real x[3], norm[3];;
    ring_xvv(n, xyz, C, /**/ x);
    ring_normal(n, xyz, C, norm);
    return vec_dot(x, norm);
}

int
ring_normal(int n, const real * xyz, const real * C, /**/ real u[3])
{
    real xu[3], xv[3], w[3];

    ring_xu(n, xyz, C, /**/ xu);
    ring_xv(n, xyz, C, /**/ xv);

    vec_cross(xu, xv, w);
    vec_norm(w, u);

    return CO_OK;
}

int
ring_wgrad(T * q, int n, const real * xyz, const real * C, /**/ real ** p)
{
    enum { X, Y, Z };
    enum { UU, UV, VV };
    int i, j;
    real g[3], xu[3], xv[3], gu[3], gv[3], g0, w[3];
    real *wgrad;
    const real *C1, *C2;

    wgrad = q->wgrad;
    C1 = C + (n + 1);
    C2 = C + 2 * (n + 1);

    ring_gcov(n, xyz, C, /**/ g);
    ring_xu(n, xyz, C, /**/ xu);
    ring_xv(n, xyz, C, /**/ xv);
    g0 = g[UU] * g[VV] - g[UV] * g[UV];
    if (g0 == 0)
        ERR(CO_NUM, "g0 = 0");

    vec_linear_combination(g[VV] / g0, xu, -g[UV] / g0, xv, /**/ gu);
    vec_linear_combination(g[UU] / g0, xv, -g[UV] / g0, xu, /**/ gv);

    for (i = j = 0; j < n + 1; j++) {
        vec_linear_combination(C1[j], gu, C2[j], gv, /**/ w);
        wgrad[i++] = w[X];
        wgrad[i++] = w[Y];

        wgrad[i++] = w[Z];
    }
    matrix_transpose(n + 1, 3, wgrad);

    *p = wgrad;
    return CO_OK;
}

real
ring_grad(int n, const real * wgrad, const real * scalar)
{
    return array_dot(n, wgrad, scalar);
}

real
ring_H(int n, const real * xyz, const real * C)
{
    real buu, buv, bvv;
    real guu, guv, gvv, g;

    guu = ring_guu(n, xyz, C);
    guv = ring_guv(n, xyz, C);
    gvv = ring_gvv(n, xyz, C);
    buu = ring_buu(n, xyz, C);
    buv = ring_buv(n, xyz, C);
    bvv = ring_bvv(n, xyz, C);
    g = guu * gvv - guv * guv;
    return -(buu * gvv - 2 * buv * guv + bvv * guu) / (2 * g);
}

real
ring_K(int n, const real * xyz, const real * C)
{
    real buu, buv, bvv;
    real guu, guv, gvv, g;

    guu = ring_guu(n, xyz, C);
    guv = ring_guv(n, xyz, C);
    gvv = ring_gvv(n, xyz, C);
    buu = ring_buu(n, xyz, C);
    buv = ring_buv(n, xyz, C);
    bvv = ring_bvv(n, xyz, C);
    g = guu * gvv - guv * guv;
    return (buu * bvv - buv * buv) / g;
}
