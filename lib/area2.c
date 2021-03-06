#include <stdio.h>
#include <math.h>
#include "real.h"
#include "co/area2.h"
#include "co/err.h"
#include "co/skel.h"
#include "co/sum.h"
#include "co/vec2.h"

real
area2(Skel * skel, const real * x, const real * y)
{
    int e, i, j, n;
    Sum *sum;
    real a[2], b[2], A;

    n = skel_ne(skel);
    sum_ini(&sum);
    for (e = 0; e < n; e++) {
        skel_edg_ij(skel, e, &i, &j);
        vec2_get(i, x, y, a);
        vec2_get(j, x, y, b);
        sum_add(sum, vec2_cross(b, a));
    }
    A = sum_get(sum);
    A = fabs(A);
    sum_fin(sum);
    return A / 2;
}
