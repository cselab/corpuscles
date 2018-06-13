#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <real.h>
#include <he/err.h>
#include <he/vec.h>
#include <he/dtri.h>
#include <he/macro.h>
#include <he/util.h>

#define FMT_IN   XE_REAL_IN
static const char **argv;

void vec(/**/ real a[3]) { vec_argv(&argv, a); }

int eq(const char *a, const char *b) { return util_eq(a, b); }
int main(__UNUSED int argc, const char **v) {
    const char *op;
    real a[3], b[3], c[3], da[3], db[3], dc[3];
    argv = v;
    argv++;
    if (*argv == NULL) ER("mssing OP");

    op = *argv++;
    if (eq(op, "angle")) {
        vec(a); vec(b); vec(c);
        dtri_angle(a, b, c, da, db, dc);
        vec_printf(da, "%g");
        vec_printf(db, "%g");
        vec_printf(dc, "%g");
    } else if (eq(op, "cot")) {
        vec(a); vec(b); vec(c);
        dtri_cot(a, b, c, da, db, dc);
        vec_printf(da, "%g");
        vec_printf(db, "%g");
        vec_printf(dc, "%g");
    } else if (eq(op, "area")) {
        vec(a); vec(b); vec(c);
        dtri_area(a, b, c, da, db, dc);
        vec_printf(da, "%g");
        vec_printf(db, "%g");
        vec_printf(dc, "%g");        
    } else        
        ER("unknown operation '%s'", op);
    return 0;
}
