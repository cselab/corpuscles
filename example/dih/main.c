#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <real.h>
#include <he/err.h>
#include <he/vec.h>
#include <he/macro.h>
#include <he/util.h>

#include <he/dih.h>
#include <he/ddih.h>

static const char **argv;

void vec(/**/ real a[3]) { vec_argv(&argv, a); }

int eq(const char *a, const char *b) { return util_eq(a, b); }
int main(__UNUSED int argc, const char **v) {
    const char *op;
    real a[3], b[3], c[3], d[3];
    real da[3], db[3], dc[3], dd[3];
    argv = v;
    argv++;
    if (*argv == NULL) ER("mssing OP");

    op = *argv++;
    if (eq(op, "angle")) {
        vec(a); vec(b); vec(c); vec(d);
        printf("%g\n", dih_angle(a, b, c, d));
    } else if (eq(op, "dangle")) {
        vec(a); vec(b); vec(c); vec(d);
        ddih_angle(a, b, c, d, /**/ da, db, dc, dd);
        vec_printf(da, "%g");
        vec_printf(db, "%g");
        vec_printf(dc, "%g");
        vec_printf(dd, "%g");
    } else
        ER("unknown operation '%s'", op);
    return 0;
}
