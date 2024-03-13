#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <real.h>
#include <co/err.h>
/* #include <co/x.h> */
#include <co/vec.h>
#include <co/tri.h>
#include <co/memory.h>
#include <co/punto.h>

real *tx, *ty, *tz, *ang, *vx, *vy, *vz;

static void
get3(int i, int j, int k, /**/ real a[3], real b[3], real c[3])
{
    vec_get(i, XX, YY, ZZ, a);
    vec_get(j, XX, YY, ZZ, b);
    vec_get(k, XX, YY, ZZ, c);
}

static void
get_edg(int i, int j, real r[3])
{
    real a[3], b[3];

    vec_get(i, XX, YY, ZZ, a);
    vec_get(j, XX, YY, ZZ, b);
    vec_minus(a, b, r);
}

static void
mesh_T(real * cot, /**/ real * H)
{
    int h, f;
    real T;

    for (h = 0; h < NH; h++) {
        T = cot[h];
        if ((f = flp(h)) != -1)
            T += cot[f];
        H[h] = T;
    }
}

static void
mesh_cot(real * H)
{
    int h, n, nn;
    int i, j, k;
    real a[3], b[3], c[3];

    for (h = 0; h < NH; h++) {
        n = nxt(h);
        nn = nxt(n);
        i = ver(h);
        j = ver(n);
        k = ver(nn);
        get3(i, j, k, /**/ a, b, c);
        H[h] = tri_cot(a, b, c);
    }
}

static void
mesh_l2(real * H)
{
    int h, n;
    int i, j;
    real r[3];

    for (h = 0; h < NH; h++) {
        n = nxt(h);
        i = ver(h);
        j = ver(n);
        get_edg(i, j, /**/ r);
        H[h] = vec_dot(r, r);
    }
}

static void
mesh_voronoi(real * T, real * l2, /**/ real * V)
{
    int h, i;

    for (h = 0; h < NH; h++) {
        i = ver(h);
        V[i] += T[h] * l2[h] / 4;
    }
}

static void
mesh_laplace(real * V0, real * T, real * A, /**/ real * V1)
{
    int h, n, i, j;

    for (h = 0; h < NH; h++) {
        n = nxt(h);
        i = ver(h);
        j = ver(n);
        V1[i] += T[h] * (V0[i] - V0[j]) / 2;
    }
    for (i = 0; i < NV; i++)
        V1[i] /= A[i];
}

static real
sum(int n, real * a)
{
    int i;
    real s;

    for (i = 0, s = 0; i < n; i++)
        s += a[i];
    return s;
}

static void
main0()
{
    RZERO(NH, &ang);
    RZERO(NH, &tx);
    RZERO(NH, &ty);
    RZERO(NH, &tz);             /* tri normal */

    RZERO(NV, &vx);
    RZERO(NV, &vy);
    RZERO(NV, &vz);             /* ver normal */

    FREE(ang);
    FREE(tx);
    FREE(ty);
    FREE(tz);
    FREE(vx);
    FREE(vy);
    FREE(vz);
}

int
main()
{
    ini("/dev/stdin");
    main0();
    fin();
    return CO_OK;
}
