#include <stdio.h>
#include <tgmath.h>

#include <real.h>

#include <he/he.h>
#include <he/memory.h>
#include <he/off.h>
#include <he/orient.h>
#include <he/ten.h>
#include <he/y.h>

struct Obj {
    int nv;
    real *x, *y, *z;
    He *he;
};
typedef struct Obj Obj;
static Obj a;

static Orient *orient;

static void main0() {
    orient_apply(orient, a.x, a.y, a.z);
    off_he_xyz_fwrite(a.he, a.x, a.y, a.z, stdout);
}

int main() {
    y_ini("/dev/stdin", &a.he, &a.x, &a.y, &a.z);
    a.nv = he_nv(a.he);
    
    orient_ini(a.he, &orient);
    main0();
    orient_fin(orient);

    y_fin(a.he, a.x, a.y, a.z);
    return 0;
}
