#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>

#include <real.h>
#include <he/err.h>
#include <he/vec.h>
#include <he/tri.h>
#include <he/macro.h>
#include <he/transform.h>
#include <he/util.h>
#include <he/y.h>

#define FMT_IN   HE_REAL_IN
static const char **argv;

static int eq(const char *a, const char *b) { return util_eq(a, b); }
int main(__UNUSED int argc, const char **v) {
    real *x, *y, *z;
    He *he;

    const char *op;
    argv = v;
    argv++;
    if (*argv == NULL)
        ER("mssing OP");
    op = *argv++;
    y_ini("/dev/stdin", &he, &x, &y, &z);
    
    if (eq(op, "centroid")) {
    } else
        ER("unknown operation '%s'", op);
    y_fin(he, x, y, z);    
    return HE_OK;
}
