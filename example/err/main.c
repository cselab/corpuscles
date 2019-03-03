#include <stdio.h>
#include <stdlib.h>

#include <real.h>
#include <co/y.h>
#include <co/err.h>

int main(void) {
    int status;
    real *x, *y, *z;
    He *he;
    err_set_ignore();
    status = y_inif(stdin, &he, &x, &y, &z);
    if (status != CO_OK)
        ER("y_inif failed");
    y_fin(he, x, y, z);
}
