#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "real.h"

#include "he/argv.h"
#include "he/err.h"
#include "he/he.h"
#include "he/memory.h"
#include "he/util.h"
#include "he/stretch.h"
#include "he/util.h"

#define T Stretch
#define FMT   HE_REAL_OUT

struct T {
    int n;
    int *plus, *minus;
    real f;
};

const static real *QX;
static int compare(const void *vi, const void *vj) {
    int *pi, *pj, i, j;
    pi = (int*)vi; pj = (int*)vj;
    i = *pi; j = *pj;
    return QX[i] < QX[j];
}

static int sort(int n, real *x, int *idx) {
    QX = x;
    qsort(idx, n, sizeof(idx[0]), compare);
    return HE_OK;
}

int stretch_argv(char ***p, He *he, real *x, real *y, real *z, /**/ T **pq) {
    T *q;
    int status, nv, n, i, *idx;
    char name[1024];
    real frac, f;

    if ((status = argv_str(p, name)) != HE_OK)
        return status;

    if (!util_eq(name, "plain"))
        ERR(HE_IO, "expecting 'plain' got '%s'", name);

    if (argv_real(p, &frac) != HE_OK)
        ERR(HE_IO, "fail to read fraction");

    if (frac > 0.5)
        ERR(HE_IO, "frac=" FMT " > 0.5", frac);

    if (argv_real(p, &f) != HE_OK)
        ERR(HE_IO, "fail to read force");

    nv = he_nv(he);
    n = (int) (nv * frac);

    MALLOC(nv, &idx);
    for (i = 0; i < nv; i++)
        idx[i] = 0;
    sort(nv, x, /**/ idx);

    MALLOC(n, &q->plus);
    MALLOC(n, &q->minus);
    for (i = 0; i < n; i++)
        q->plus[i] = idx[i];
    for (i = 0; i < n; i++)
        q->plus[i] = idx[nv - 1 - i];

    FREE(idx);

    MALLOC(1, &q);
    q->n = n;
    q->f = f;
    *pq = q;

    return HE_OK;
}

int stretch_fin(T *q) {
    FREE(q);
    return HE_OK;
}

int stretch_force(T *q, const real *x, const real *y, const real *z, /*io*/ real *fx, real *fy, real *fz) {
    return HE_OK;
}

int stretch_n(T *q) { return q->n; }
real stretch_f(T *q) { return q->f; }
