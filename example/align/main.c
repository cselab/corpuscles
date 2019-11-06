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
#include <co/ten.h>
#include <co/vec.h>
#include <co/util.h>
#include <co/y.h>

const char *me = "co.align";

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
    argv = v; argv++;
    if (argv && eq(argv, "-h"))
        usg();
    arg();

    y_ini(a.file, &a.he, &a.x, &a.y, &a.z);
    a.n = he_nv(a.he);

    y_ini(b.file, &b.he, &b.x, &b.y, &b.z);
    b.n = he_nv(b.he);

    orient_ini(a.he, &a.orient);
    orient_ini(b.he, &b.orient);

    orient_apply(a.orient, a.x, a.y, a.z);
    orient_apply(b.orient, b.x, b.y, b.z);

    orient_transform(a.orient, &a.t);
    orient_transform(b.orient, &b.t);

    invert(&a.t, b.n, b.x, b.y, b.z);
    off_he_xyz_fwrite(b.he, b.x, b.y, b.z, stdout);

    orient_fin(a.orient);
    orient_fin(b.orient);

    y_fin(a.he, a.x, a.y, a.z);
    y_fin(b.he, b.x, b.y, b.z);
    return 0;
}
