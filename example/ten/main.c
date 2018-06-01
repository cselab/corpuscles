#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <real.h>
#include <he/err.h>
#include <he/vec.h>
#include <he/ten.h>
#include <he/macro.h>
#include <he/util.h>

static const char **argv;
void vec(/**/ real a[3]) { vec_argv(&argv, a); }
void ten(/**/ Ten *T) { ten_argv(&argv, T); }

int eq(const char *a, const char *b) { return util_eq(a, b); }
int main(__UNUSED int c, const char **v) {
    argv = v;
    
    const char *op;
    real a[3], b[3];
    Ten T, R, P, Q;
    
    argv++;
    if (*argv == NULL) ER("mssing OP");

    op = *argv++;
    if (eq(op, "dyadic")) {
        vec(a);
        vec(b);
        ten_dyadic(a, b, &T);
        ten_printf(T, "%g");
    } else if (eq(op, "plus")) {
        ten(&T);
        ten(&R);
        ten_plus(T, R, &P);
        ten_printf(P, "%g");
    } else
        ER("unknown operation '%s'", v[0]);
    return 0;
}
