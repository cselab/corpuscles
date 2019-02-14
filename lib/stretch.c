#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "real.h"

#include "he/argv.h"
#include "he/err.h"
#include "he/he.h"
#include "he/macro.h"
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

static int sort(int n, const real *x, int *idx) {
    QX = x;
    qsort(idx, n, sizeof(idx[0]), compare);
    return HE_OK;
}

static int plain(int nv, const real *x, __UNUSED const real *y, char ***p, T *q) {
    int n, i, *idx;
    int *plus, *minus;
    real frac, f;

    if (argv_real(p, &f) != HE_OK)
        ERR(HE_IO, "fail to read force");
    if (argv_real(p, &frac) != HE_OK)
        ERR(HE_IO, "fail to read fraction");
    if (frac > 0.5)
        ERR(HE_IO, "frac=" FMT " > 0.5", frac);

    n = (int) (nv * frac);

    MALLOC(nv, &idx);
    for (i = 0; i < nv; i++)
        idx[i] = i;
    sort(nv, x, /**/ idx);

    MALLOC(n, &plus);
    MALLOC(n, &minus);
    for (i = 0; i < n; i++)
        plus[i] = idx[i];
    for (i = 0; i < n; i++)
        minus[i] = idx[nv - 1 - i];

    FREE(idx);

    q->n = n;
    q->f = f;
    q->minus = minus;
    q->plus = plus;

    return HE_OK;
}

int stretch_argv(char ***p, He *he, real *x, real *y, real *z, /**/ T **pq) {
    T *q;
    int nv, status;
    char name[1024];

    nv = he_nv(he);
    MALLOC(1, &q);

    if ((status = argv_str(p, name)) != HE_OK)
        return status;

    if (!util_eq(name, "plain"))
        ERR(HE_IO, "expecting 'plain' got '%s'", name);

    plain(nv, x, y, p, q);

    *pq = q;
    return HE_OK;
}

int stretch_fin(T *q) {
    FREE(q->minus);
    FREE(q->plus);
    FREE(q);
    return HE_OK;
}

int stretch_force(T *q, const real *x, const real *y, const real *z, /*io*/ real *fx, __UNUSED real *fy, __UNUSED real *fz) {
    int i, j, n;
    real f;

    n = q->n;
    f = q->f;

    for (i = 0; i < n; i++) {
        j = q->plus[i];
        fx[j] += f/n;
    }

    for (i = 0; i < n; i++) {
        j = q->minus[i];
        fx[j] -= f/n;
    }

    return HE_OK;
}

int stretch_n(T *q) { return q->n; }
real stretch_f(T *q) { return q->f; }
