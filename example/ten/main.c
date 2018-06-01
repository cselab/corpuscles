#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <real.h>
#include <he/err.h>
#include <he/vec.h>
#include <he/ten.h>
#include <he/macro.h>
#include <he/util.h>

int eq(const char *a, const char *b) { return util_eq(a, b); }
int main(__UNUSED int c, const char *v[]) {
    real a[3], b[3];
    Ten T, R, P, Q;

    v++; vec_argv(&v, a);
    vec_printf(a, "%-.6g");
    return 0;
    
    v++;
    if (v[0] == NULL) ER("mssing OP");
    
    if (eq(v[0], "dyadic")) {
        v++; vec_argv(&v, a);
    } else
        ER("unknown operation '%s'", v[0]);
    
    return 0;
}
