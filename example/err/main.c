#include <stdio.h>
#include <stdlib.h>

#include <real.h>
#include <he/y.h>
#include <he/err.h>

int main(void) {
    int status;
    real *x, *y, *z;
    He *he;
    err_set_ignore();

    status = y_inif(stdin, &he, &x, &y, &z);
    if (status != HE_OK)
        ER("y_inif failed");
    y_fin(he, x, y, z);
}
