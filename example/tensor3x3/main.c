#include <stdio.h>
#include <real.h>
#include <co/err.h>
#include <co/memory.h>
#include <co/matrix.h>
#include <co/tensor3x3.h>

#define FMT CO_REAL_OUT

enum {XX, XY, XZ, YX, YY, YZ, ZX, ZY, ZZ};

int
main(void)
{
    int i;
    int j;
    int n;
    struct Tensor3x3 q;
    real **d;
    real *x;
    real *y;
    real *z;
    real *u;
    real *v;
    real *w;
    real s;

    n = 2;
    tensor3x3_ini(n, &q);

    d = q.d;
    for (i = 0; i < n; i ++)
	for (j = 0; j < n; j ++) {
	    matrix_set(n, n, i, j, 1 + i*j, d[XX]);
	    matrix_set(n, n, i, j, 2 + i*j, d[XY]);
	    matrix_set(n, n, i, j, 3 + i*j, d[XZ]);
	    matrix_set(n, n, i, j, 4 + i*j, d[YX]);
	    matrix_set(n, n, i, j, 5 + i*j, d[YY]);
	    matrix_set(n, n, i, j, 6 + i*j, d[YZ]);
	    matrix_set(n, n, i, j, 7 + i*j, d[ZX]);
	    matrix_set(n, n, i, j, 8 + i*j, d[ZY]);
	    matrix_set(n, n, i, j, 9 + i*j, d[ZZ]);
	}

    MALLOC3(n, &x, &y, &z);
    CALLOC3(n, &u, &v, &w);

    s = 10;
    for (i = 0; i < n; i++) {
	x[i] = (i + 1) * 1;
	y[i] = (i + 1) * 10;
	z[i] = (i + 1) * 100000;
    }
    tensor3x3_vector(n, s, x, y, z, &q, u, v, w);
    for (i = 0; i < n; i++)
	printf(FMT " " FMT " " FMT "\n", u[i], v[i], w[i]);

    FREE3(x, y, z);
    FREE3(u, v, w);
    tensor3x3_fin(&q);
}
