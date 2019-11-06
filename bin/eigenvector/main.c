#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>
#include <real.h>
#include <co/eigen.h>
#include <co/err.h>
#include <co/he.h>
#include <co/macro.h>
#include <co/transform.h>
#include <co/vec.h>
#include <co/off.h>
#include <co/y.h>
#define FMT CO_REAL_OUT
const char me[] = "co.eigenvector";
const real pi = 3.141592653589793;

static int mult(const real[3 * 3], real *, real *, real *);
static int eigen_vector_point(Eigen *, He *, real *, real *, real *, real *);

static void
usg(void)
{
    fprintf(stderr, "%s [-sptv] < C.off\n", me);
    fprintf(stderr, "  -t   use triangles\n");
    fprintf(stderr, "  -s   use surface\n");
    fprintf(stderr, "  -p   use points (default)\n");
    fprintf(stderr, "  -a   print eigenvalues (default: eigenvector)");
    exit(2);
}

int
main(__UNUSED int argc, char **argv)
{
    enum { X, Y, Z };
    enum { VECTOR, VALUE, SHAPE };
    int Report, i, n;
    real *x, *y, *z, vec[3 * 3], val[3];
    real *u, *v, *w;
    real a, b, c, A, B, C;
    He *he, *she;
    Eigen *eigen;
    const char *shape;

    int (*Alg)(Eigen *, He *, real *, real *, real *, real *);
    const char *arg;

    argv++;
    Alg = eigen_vector_point;
    Report = VECTOR;
    while (*argv != NULL) {
	arg = argv++[0];
	if (arg[0] != '-')
	    break;
	switch (arg[1]) {
	case 'h':
	    usg();
	    break;
	case 's':
	    Alg = eigen_vector_surface;
	    break;
	case 'p':
	    Alg = eigen_vector_point;
	    break;
	case 't':
	    Alg = eigen_vector_tri;
	    break;
	case 'a':
	    Report = VALUE;
	    break;
	case 'i':
	    Report = SHAPE;
	    if ((shape = *argv++) == NULL)
		ER("-i requires an argument");
	    break;
	default:
	    ER("%s: unknown option '%s'", me, arg);
	}
    }

    y_inif(stdin, &he, &x, &y, &z);
    eigen_ini(he, &eigen);
    Alg(eigen, he, x, y, z, vec);
    eigen_values(eigen, val);

    switch (Report) {
    case VECTOR:
	printf(FMT " " FMT " " FMT "\n", vec[0], vec[3], vec[6]);
	break;
    case VALUE:
	vec_printf(val, FMT);
	break;
    case SHAPE:
	y_ini(shape, &she, &u, &v, &w);
	A = val[X];
	B = val[Y];
	C = val[Z];
	a = C + B - A;
	b = C - B + A;
	c = - (C - B - A);

	a = sqrt(a);
	b = sqrt(b);
	c = sqrt(c);

	MSG("ABC: %g %g %g", a, b, c);
	n = he_nv(she);
	transform_scalx(a, n, u, v, w);
	transform_scaly(b, n, u, v, w);
	transform_scalz(c, n, u, v, w);
	for (i = 0; i < n; i++)
	    mult(vec, &u[i], &v[i], &w[i]);
	off_he_xyz_fwrite(she, u, v, w, stdout);
	y_fin(she, u, v, w);
	break;
    }
    eigen_fin(eigen);
    y_fin(he, x, y, z);
    return 0;
}

static int
mult(const real m[3 * 3], real * px, real * py, real * pz)
{
    enum { XX, XY, XZ, YX, YY, YZ, ZX, ZY, ZZ };
    real x, y, z, x0, y0, z0;

    x = *px;
    y = *py;
    z = *pz;

    x0 = x * m[XX] + y * m[XY] + z * m[XZ];
    y0 = x * m[YX] + y * m[YY] + z * m[YZ];
    z0 = x * m[ZX] + y * m[ZY] + z * m[ZZ];

    *px = x0;
    *py = y0;
    *pz = z0;
    return CO_OK;
}

static int
eigen_vector_point(Eigen * q, He * he, real * x, real * y, real * z,
		   real * v)
{
    USED(he);
    return eigen_vector(q, x, y, z, v);
}
