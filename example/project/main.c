#include <stdio.h>
#include <tgmath.h>

static double a = 0.54353, b = 0.121435, c = -0.561365;
static double pi = 3.141592653589793;

struct Point
{
    double x, y;
};

static double
F(double q)
{
    return c*pow(q, 5)+b*pow(q, 3)+a*q;
}

static int
q2xy(double q, double *x, double *y)
{
    *x = sin(q);
    *y = F(cos(q));
    return 0;
}

static double
dist2(double q, void *p) {
    struct Point *point;
    double x, y, dx, dy;

    point = p;
    q2xy(q, &x, &y);
    dx = point->x - x;
    dy = point->y - y;
    return dx*dx + dy*dy;
}

static double
min(double x, double y, double *px, double *py) {
    double lo, hi, q, h, d;
    double p, m;
    int NegX, NegY;
    struct Point point;
    int i, n;
    lo = 0;
    hi = pi/2;
    n = 1000;
    h = (hi - lo)/(n - 1);

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
	q = lo + i*h;
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

int min3(const double r[3], /**/ double s[3]) {
    enum {X, Y, Z};
    double x, y, p, u, v;
    x = sqrt(r[X]*r[X] + r[Y]*r[Y]);
    y = r[Y];
    p = atan2(r[Y], r[X]);

    min(x, y, &u, &v);

    s[Z] = v;
    s[X] = u*cos(p);
    s[Y] = u*sin(p);

    return 0;
}

int
main()
{
    double lo, hi, q, x, y, x0, y0;
    int i, n;
    lo = 0;
    hi = pi/2;
    n = 20;

    for (i = 0; i < n; i++) {
	q = lo + i*(hi - lo)/(n - 1);
	q2xy(q, &x, &y);
	printf("%g %g\n", x, y);
    }

    min(x = 0.5, y = 0.0, &x0, &y0);
    printf("%g %g\n", x, y);
    printf("%g %g\n", x0, y0);    
}
