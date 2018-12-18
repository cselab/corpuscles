#include <stdio.h>
#include <tgmath.h>

#include <real.h>

#include <he/err.h>
#include <he/he.h>
#include <he/memory.h>
#include <he/off.h>
#include <he/orient.h>
#include <he/ten.h>
#include <he/vec.h>
#include <he/y.h>

struct Obj {
    int n;
    real *x, *y, *z;
    Ten t;
    He *he;
};
typedef struct Obj Obj;
static Obj a;

static Orient *orient;

static int invert(int n, const Ten *t, real *x, real *y, real *z) {
    int i;
    real v[3], u[3];
    for (i = 0; i < n; i++) {
        vec_get(i, x, y, z, v);
        ten_vec(t, v, u);
        vec_set(u, i, x, y, z);
    }
    return HE_OK;
}

static void main0(void) {
    orient_apply(orient, a.x, a.y, a.z);
    orient_transform(orient, &a.t);
    //ten_printf(&a.t, "%g");
    invert(a.n, &a.t, a.x, a.y, a.z);
    off_he_xyz_fwrite(a.he, a.x, a.y, a.z, stdout);
}

int main() {
    y_ini("/dev/stdin", &a.he, &a.x, &a.y, &a.z);
    a.n = he_nv(a.he);
    
    orient_ini(a.he, &orient);
    main0();
    orient_fin(orient);

    y_fin(a.he, a.x, a.y, a.z);
    return 0;
}
