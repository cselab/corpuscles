#include <stdio.h>
#include <stdlib.h>

#include <real.h>
#include <co/memory.h>
#include <co/off.h>
#include <co/he.h>
#include <co/err.h>
#include <co/util.h>
#include <co/macro.h>
#include <co/y.h>

static const char *me = "co.orient";

static void usg(void) {
    fprintf(stderr, "%s -t x y z -r ox oy oz -f [field of view] < IN.off > OUT.off\n", me);
    exit(2);
}

static int eq(const char **a, const char *b) {
    return (*a != NULL) && util_eq(*a, b);
};

int main(__UNUSED int c, const char **v) {
    real *x, *y, *z;
    He *he;

    v++;
    if (eq(v, "-h"))
	usg();

    if (y_inif(stdin, &he, &x, &y, &z) != CO_OK)
	ER("fail to open input file");
    off_he_xyz_fwrite(he, x, y, z, stdout);
    y_fin(he, x, y, z);
    return 0;
}
