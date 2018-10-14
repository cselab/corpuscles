#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <real.h>
#include <he/err.h>
#include <he/vec.h>
#include <he/dedg.h>
#include <he/macro.h>
#include <he/util.h>

static const char **argv;

void vec(/**/ real a[3]) { vec_argv(&argv, a); }

int eq(const char *a, const char *b) { return util_eq(a, b); }
int main(__UNUSED int argc, const char **v) {
    const char *op;
    real a[3], b[3], da[3], db[3];
    argv = v;
    argv++;
    if (*argv == NULL) ER("mssing OP");

    op = *argv++;
    if (eq(op, "abs")) {
        vec(a); vec(b);
        dedg_abs(a, b, da, db);
        vec_printf(da, "%g");
        vec_printf(db, "%g");
    } else
        ER("unknown operation '%s'", op);
    return 0;
}
