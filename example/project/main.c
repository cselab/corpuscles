#include <stdio.h>
#include <math.h>
#include <real.h>
#include <co/y.h>
#include <co/he.h>
#include <co/vec.h>
#include <co/off.h>
#include <co/force.h>
#include <co/array.h>
#include <co/memory.h>
#include <co/err.h>
#include <co/equiangulate.h>
#include <co/macro.h>

static double a = 0.54353, b = 0.121435, c = -0.561365;
static double pi = 3.141592653589793;

struct Point {
    double x, y;
};

static double
F(double q)
{
    return c * pow(q, 5) + b * pow(q, 3) + a * q;
}

static int
q2xy(double q, double *x, double *y)
{
    *x = sin(q);
    *y = F(cos(q));
    return 0;
}

static double
dist2(double q, void *p)
{
    struct Point *point;
    double x, y, dx, dy;

    point = p;
    q2xy(q, &x, &y);
    dx = point->x - x;
    dy = point->y - y;
    return dx * dx + dy * dy;
}

static double
min(double x, double y, double *px, double *py)
{
    double lo, hi, q, h, d;
    double p, m;
    int NegX, NegY;
    struct Point point;
    int i, n;

    lo = 0;
    hi = pi / 2;
    n = 100;
    h = (hi - lo) / (n - 1);

    NegX = NegY = 0;
    if (x < 0) {
	NegX = 1;
	x = -x;
    }

    if (y < 0) {
	NegY = 1;
	y = -y;
    }

    point.x = x;
    point.y = y;
    p = 0;
    m = 1e42;
    for (i = 0; i < n; i++) {
	q = lo + i * h;
	d = dist2(q, &point);
	if (d < m) {
	    m = d;
	    p = q;
	}
    }
    q2xy(p, &x, &y);
    if (NegX)
	x = -x;
    if (NegY)
	y = -y;

    *px = x;
    *py = y;

    return 0;
}

int
min3(const double r[3], /**/ double s[3])
{
    enum { X, Y, Z };
    double x, y, p, u, v;

    x = sqrt(r[X] * r[X] + r[Y] * r[Y]);
    y = r[Z];
    p = atan2(r[Y], r[X]);
    min(x, y, &u, &v);
    s[X] = u * cos(p);
    s[Y] = u * sin(p);
    s[Z] = v;
    return 0;
}

static int
project(int n, double *x, double *y, double *z)
{
    int i;
    double r[3], s[3];

    for (i = 0; i < n; i++) {
	vec_get(i, x, y, z, r);
	min3(r, s);
	vec_set(s, i, x, y, z);
    }
    return 0;
}

static int
project_avg(double f, int n, double *x, double *y, double *z)
{
    int i;
    double r[3], s[3], u[3];

    for (i = 0; i < n; i++) {
	vec_get(i, x, y, z, r);
	min3(r, s);
	vec_linear_combination(f, s, 1 - f, r, u);
	vec_set(u, i, x, y, z);
    }
    return 0;
}

int
main(int argc, char **argv)
{
    USED(argc);
    enum { X, Y, Z };
    double *x, *y, *z, *fx, *fy, *fz;
    He *he;
    Force *force;
    int i, nv, ne;
    double dt;
    y_inif(stdin, &he, &x, &y, &z);
    nv = he_nv(he);
    ne = he_ne(he);
    MALLOC3(nv, &fx, &fy, &fz);

    argv++;
    force_argv("harmonic", &argv, he, &force);
    dt = 0.01;
    for (i = 0; i < 1000; i++) {
	array_zero3(nv, fx, fy, fz);
	force_force(force, he, x, y, z, fx, fy, fz);
	array_axpy3(nv, -dt, fx, fy, fz, x, y, z);
	project(nv, x, y, z);
	if (i % 100 == 0) {
	    MSG("eng: %g", force_energy(force, he, x, y, z) / ne * 100);
	}
    }
    project(nv, x, y, z);
    off_he_xyz_fwrite(he, x, y, z, stdout);
    y_fin(he, x, y, z);
    force_fin(force);
    FREE3(fx, fy, fz);
}
