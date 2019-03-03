#include <stdio.h>

#include <real.h>
#include <co/err.h>
#include <co/he.h>
#include <co/normal.h>
#include <co/dnormal.h>
#include <co/memory.h>
#include <co/punto.h>
#include <co/sum.h>
#include <co/ten.h>
#include <co/y.h>

int main() {
    Dnormal *dnormal;
    He *he;
    int n, i;
    real *x, *y, *z;
    Ten *f;

    y_inif(stdin, &he, &x, &y, &z);
    n = he_nv(he);
    dnormal_ini(he, &dnormal);
    dnormal_apply(dnormal, he, x, y, z, /**/ &f);

    puts("x y z xx xy xz yx yy yz zx zy zz trace determinant");
    for (i = 0; i < n; i++) {
        printf("%g %g %g ", x[i], y[i], z[i]);
        ten_line(&f[i]); printf(" ");
        printf("%.16g %.16g",
               ten_trace(&f[i]), ten_determinant(&f[i]));
        puts("");
    }

    dnormal_fin(dnormal);
    y_fin(he, x, y, z);
    return CO_OK;
}
