#include <stdio.h>

#include <real.h>
#include <he/err.h>
#include <he/obj.h>

#include <he/y.h>

static real *x, *y, *z;
static He *he;

int main() {
    y_inif(stdin, &he, &x, &y, &z);
    obj_fwrite(he, x, y, z, stdout);
    y_fin(he, x, y, z);
    return 0;
}
