#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <real.h>
#include <he/err.h>
#include <he/vec.h>
#include <he/ten.h>
#include <he/dvec.h>
#include <he/macro.h>
#include <he/util.h>

#define FMT_IN   XE_REAL_IN
static const char **argv;

void vec(/**/ real a[3]) { vec_argv(&argv, a); }
int eq(const char *a, const char *b) { return util_eq(a, b); }
int main(__UNUSED int argc, const char **v) {
    const char *op;
    real a[3];
    Ten t;
    argv = v;
    argv++;
    if (*argv == NULL) ER("mssing OP");

    op = *argv++;
    if (eq(op, "norm")) {
        vec(a);
        dvec_norm(a, &t);
        ten_printf(&t, "%g");
    } else
        ER("unknown operation '%s'", op);
    return 0;
}
