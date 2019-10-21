#include <tgmath.h>
#include <stdio.h>
#include "real.h"
#include "co/array.h"
#include "co/edg2.h"
#include "co/err.h"
#include "co/len.h"
#include "co/skel.h"
#include "co/sum.h"
#include "co/vec2.h"

real
len(Skel * skel, const real * x, const real * y)
{
    int e, i, j, n;
    HeSum *sum;
    real a[2], b[2], L;

    n = skel_ne(skel);
    he_sum_ini(&sum);
    for (e = 0; e < n; e++) {
        skel_edg_ij(skel, e, &i, &j);
        vec2_get(i, x, y, a);
        vec2_get(j, x, y, b);
        he_sum_add(sum, edg2_abs(a, b));
    }
    L = he_sum_get(sum);
    he_sum_fin(sum);
    return L;
}

int
len_ver(Skel * skel, const real * x, const real * y, real * ans)
{
    int e, i, j, ne, nv;
    real a[2], b[2], l;

    ne = skel_ne(skel);
    nv = skel_nv(skel);
    array_zero(nv, ans);
    for (e = 0; e < ne; e++) {
        skel_edg_ij(skel, e, &i, &j);
        vec2_get(i, x, y, a);
        vec2_get(j, x, y, b);
        l = edg2_abs(a, b) / 2;
        ans[i] += l;
        ans[j] += l;
    }
    return CO_OK;
}

int
len_edg(Skel * skel, const real * x, const real * y, real * ans)
{
    int e, i, j, n;
    real a[2], b[2];

    n = skel_ne(skel);
    for (e = 0; e < n; e++) {
        skel_edg_ij(skel, e, &i, &j);
        vec2_get(i, x, y, a);
        vec2_get(j, x, y, b);
        ans[e] = edg2_abs(a, b);
    }
    return CO_OK;
}
