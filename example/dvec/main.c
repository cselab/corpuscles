#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <real.h>
#include <co/err.h>
#include <co/vec.h>
#include <co/ten.h>
#include <co/dvec.h>
#include <co/macro.h>
#include <co/util.h>

#define FMT_IN   CO_REAL_IN
static const char **argv;

static void vec(/**/ real a[3]) { vec_argv(&argv, a); }
static int eq(const char *a, const char *b) { return util_eq(a, b); }
int main(__UNUSED int argc, const char **v) {
    const char *op;
    real a[3], b[3];
    Ten Da, Db;
    Ten t;
    argv = v;
    argv++;
    if (*argv == NULL) ER("mssing OP");

    op = *argv++;
    if (eq(op, "norm")) {
        vec(a);
        dvec_norm(a, &t);
        ten_printf(&t, "%g");
    } else if (eq(op, "minus")) {
        vec(a); vec(b);
        dvec_minus(a, b, /**/ &Da, &Db);
        ten_printf(&Da, "%g");
        ten_printf(&Db, "%g");
    } else
        ER("unknown operation '%s'", op);
    return 0;
}
