#include <tgmath.h>
#include <stdio.h>

#include "real.h"
#include "co/he.h"
#include "co/vec.h"
#include "co/edg.h"
#include "co/sum.h"
#include "co/segment.h"

real
segment_average(He * he, const real * x, const real * y, const real * z)
{
    HeSum *sum;
    int n, e, i, j;
    real a[3], b[3], s;

    n = he_ne(he);
    he_sum_ini(&sum);
    for (e = 0; e < n; e++) {
        he_edg_ij(he, e, &i, &j);
        vec_get(i, x, y, z, a);
        vec_get(j, x, y, z, b);
        he_sum_add(sum, edg_abs(a, b));
    }
    s = he_sum_get(sum);
    he_sum_fin(sum);
    return s / n;
}
