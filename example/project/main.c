#include <stdio.h>
#include <tgmath.h>
#include <real.h>
#include <co/y.h>
#include <co/he.h>
#include <co/vec.h>
#include <co/off.h>

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
    n = 1000;
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

int
main()
{
    enum { X, Y, Z };
    double *x, *y, *z, r[3], s[3];
    He *he;
    int i, nv;

    y_inif(stdin, &he, &x, &y, &z);

    nv = he_nv(he);
    for (i = 0; i < nv; i++) {
	vec_get(i, x, y, z, r);
	min3(r, s);
	vec_set(s, i, x, y, z);
    }
    off_he_xyz_fwrite(he, x, y, z, stdout);
    y_fin(he, x, y, z);
}
