#include <stdio.h>
#include <stdlib.h>

#include <real.h>
#include <he/y.h>
#include <he/err.h>

int main(void) {
    real *x, *y, *z;
    He *he;
    err_set_ignore();
    y_inif(stdin, &he, &x, &y, &z);
    y_fin(he, x, y, z);
}
