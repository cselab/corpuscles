#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <real.h>

#include <co/err.h>
#include <co/he.h>
#include <co/macro.h>
#include <co/memory.h>
#include <co/off.h>
#include <co/orient.h>
#include <co/memory.h>
#include <co/ten.h>
#include <co/vec.h>
#include <co/util.h>
#include <co/y.h>

const char *me = "co.align";

const static int perms[][3] = {
    {0, 0, 0}, {0, 0, 1},
    {0, 1, 0}, {0, 1, 1},
    {1, 0, 0}, {1, 0, 1},
    {1, 1, 0}, {1, 1, 1}};

static void usg(void) {
    fprintf(stderr, "%s A.off B.off > C.off\n", me);
    fprintf(stderr, "align B.off with A.off\n");
    exit(2);
}

static const char **argv;
struct Obj {
    int n;
    real *x, *y, *z;
    Ten t;
    He *he;
    char file[4048];
    Orient *orient;
};

typedef struct Obj Obj;
static Obj a, b;
static real *xb, *yb, *zb; /* backup */

static int invert(const Ten *t, int n, real *x, real *y, real *z) {
    int i;
    real v[3], u[3];
    for (i = 0; i < n; i++) {
        vec_get(i, x, y, z, v);
        ten_vec(t, v, u);
        vec_set(u, i, x, y, z);
    }
    return CO_OK;
}

static int push(void) {
    int i;
    for (i = 0; i < b.n; i++) {
        xb[i] = b.x[i];
        yb[i] = b.y[i];
        zb[i] = b.z[i];
    }
    return CO_OK;
}

static int pop(void) {
    int i;
    for (i = 0; i < b.n; i++) {
        b.x[i] = xb[i];
        b.y[i] = yb[i];
        b.z[i] = zb[i];
    }
    return CO_OK;
}

static real sq(real x) { return x*x; };
static real dist(void) {
    int i, j;
    real d, s, m;
    s = m = 0;
    for (i = 0; i < a.n; i++) {
        m = 1e32;
        for (j = 0; j < b.n; j++) {
            d = sq(a.x[i] - b.x[j]) +
                sq(a.y[i] - b.y[j]) +
                sq(a.z[i] - b.z[j]);
            if (d < m) m = d;
        }
        s += m;
    }
    return s;
}

static int flip0(int n, real *x) {
    int i;
    for (i = 0; i < n; i++)
        x[i] = -x[i];
}

static int flip(const int perm[3]) {
    enum {X, Y, Z};
    if (perm[X]) flip0(b.n, b.x);
    if (perm[Y]) flip0(b.n, b.y);
    if (perm[Z]) flip0(b.n, b.z);
}

static int str(/**/ char *p) {
    if (*argv == NULL)
        ER("not enough arguments");
    strncpy(p, *argv, 4047);
    argv++;
    return CO_OK;
}

static int arg() {
    str(a.file);
    str(b.file);
}

static int eq(const char **a, const char *b) {
    return (*a != NULL) && util_eq(*a, b);
};

int main(__UNUSED int argc, const char **v) {
    int i, j, n;
    real d, m;

    argv = v; argv++;
    if (argv && eq(argv, "-h"))
        usg();
    arg();

    y_ini(a.file, &a.he, &a.x, &a.y, &a.z);
    a.n = he_nv(a.he);

    y_ini(b.file, &b.he, &b.x, &b.y, &b.z);
    b.n = he_nv(b.he);
    MALLOC(b.n, &xb);
    MALLOC(b.n, &yb);
    MALLOC(b.n, &zb);

    orient_ini(a.he, &a.orient);
    orient_ini(b.he, &b.orient);

    orient_apply(a.orient, a.x, a.y, a.z);
    orient_apply(b.orient, b.x, b.y, b.z);

    orient_transform(a.orient, &a.t);
    orient_transform(b.orient, &b.t);

    m = 1e32;
    n = sizeof(perms)/sizeof(perms[0]);

    push();
    for (j = i = 0; i < n; i++) {
        flip(perms[i]);
        d = dist();
        if (d < m) {
            m = d;
            j = i;
        }
        pop();
    }
    flip(perms[j]);
    invert(&a.t, b.n, b.x, b.y, b.z);
    off_he_xyz_fwrite(b.he, b.x, b.y, b.z, stdout);

    orient_fin(a.orient);
    orient_fin(b.orient);

    y_fin(a.he, a.x, a.y, a.z);
    y_fin(b.he, b.x, b.y, b.z);
    FREE(xb); FREE(yb); FREE(zb);

    return 0;
}
