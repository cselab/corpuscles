#include <stdio.h>

#include <real.h>
#include <he/err.h>
#include <he/he.h>
#include <he/normal.h>
#include <he/dnormal.h>
#include <he/memory.h>
#include <he/punto.h>
#include <he/sum.h>
#include <he/ten.h>
#include <he/y.h>

int main() {
    Dnormal *dnormal;
    He *he;
    int n;
    real *x, *y, *z;
    Ten *f;

    y_ini("/dev/stdin", &he, &x, &y, &z);
    n = he_nv(he);
    dnormal_ini(he, &dnormal);
    dnormal_apply(dnormal, he, x, y, z, /**/ &f);

    dnormal_fin(dnormal);
    y_fin(he, x, y, z);
    return HE_OK;
}
