#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>

#include "real.h"

#include "he/argv.h"
#include "he/array.h"
#include "he/err.h"
#include "he/he.h"
#include "he/macro.h"
#include "he/memory.h"
#include "he/util.h"
#include "he/stretch.h"
#include "he/util.h"

#define T Stretch
#define FMT HE_REAL_OUT
#define SIZE(a) (sizeof(a)/sizeof((a)[0]))

#define MAX_ITER (100)

struct T {
    int n;
    int *plus, *minus;
    real f;
};

typedef int (*IniFunType)(int, const real*, const real*, char***, T*);
static int plain(int, const real*, const real*, char***, T*);
static int cylinder(int, const real*, const real*, char***, T*);
static const char *IniName[] = {"plain", "cylinder"};
static const IniFunType IniFun[] = {plain, cylinder};

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


static struct {
    int n;
    real x0;
    const real *xx, *yy;
} COUNT_DATA;
#   define GET(k) k = COUNT_DATA.k
#   define SET(k, v) COUNT_DATA.k = (v)
static int count(real r0) {
#   define sq(x) ((x)*(x))
    real x0;
    int n;
    const real *xx, *yy;
    int i, cnt;
    real x, y, r, rsq;
    GET(x0); GET(n); GET(xx); GET(yy);
    cnt = 0;
    r0 = sq(r0);
    for (i = 0; i < n; i++) {
        x = xx[i]; y = yy[i];
        r = sq(x - x0) + sq(y);
        if (r < r0)
            cnt++;
    }
    return cnt;
#   undef sq
}

static int select0(real r0, int *a) {
#   define sq(x) ((x)*(x))
    real x0;
    int n;
    const real *xx, *yy;
    int i, cnt;
    real x, y, r, rsq;
    GET(x0); GET(n); GET(xx); GET(yy);
    cnt = 0;
    r0 = sq(r0);
    for (i = 0; i < n; i++) {
        x = xx[i]; y = yy[i];
        r = sq(x - x0) + sq(y);
        if (r < r0)
            a[cnt++] = i;
    }
    return HE_OK;
#   undef sq
}

static real get_rmax(int n, const real *x, const real *y) {
    real a, b;
    a = array_max(n, x) - array_min(n, x);
    b = array_max(n, y) - array_min(n, y);
    return a > b ? a : b;
}

static real bin_search(int n, real h) {
    real l, m;
    int i, c;
    for (l = 0, i = 0; i < MAX_ITER; i++) {
        m = (l + h) / 2;
        c = count(m);
        if (c == n) break;
        else if (c < n) l = m;
        else h = m;
    }
    return m;
}

static int cylinder(int nv, const real *x, __UNUSED const real *y, char ***p, T *q) {
    real f, frac, x0, rmax;
    int *plus, *minus;
    int n;

    if (argv_real(p, &f) != HE_OK)
        ERR(HE_IO, "fail to read force");
    if (argv_real(p, &frac) != HE_OK)
        ERR(HE_IO, "fail to read fraction");
    if (frac > 0.5)
        ERR(HE_IO, "frac=" FMT " > 0.5", frac);
    if (argv_real(p, &x0) != HE_OK)
        ERR(HE_IO, "fail to read x0");

    q->f = f;
    n = (int)(nv*frac);

    /* set data for count() */
    rmax = fabs(x0) + get_rmax(nv, x, y);
    SET(n, nv); SET(xx, x); SET(yy, y);
    
    real r;
    SET(x0, x0); r = bin_search(n, rmax);
    n = count(r);
    MALLOC(n, &plus);
    MALLOC(n, &minus);
    select0(r, plus);
    SET(x0, -x0); r = bin_search(n, rmax);
    if (n != count(r))
        ERR(HE_IO, "n=%d != count(rmax)=%d", n, count(rmax));
    select0(r, minus);

    q->minus = minus;
    q->plus = plus;
    q->n  = n;

    return HE_OK;
}

int stretch_argv(char ***p, He *he, real *x, real *y, real *z, /**/ T **pq) {
    T *q;
    int i, nv, status;
    char name[1024];

    nv = he_nv(he);
    MALLOC(1, &q);

    if ((status = argv_str(p, name)) != HE_OK)
        return status;

    i = 0;
    for (i = 0; ; i++) {
        if (i == SIZE(IniName))
            ERR(HE_IO, "expecting 'plain' or 'cyliner' got '%s'", name);
        if (util_eq(name, IniName[i])) {
            status = IniFun[i](nv, x, y, p, q);
            if (status != HE_OK)
                return status;
            break;
        }
    }

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
