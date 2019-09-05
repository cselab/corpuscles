#include <stdio.h>
#include <tgmath.h>

static double a = 0.54353, b = 0.121435, c = -0.561365;

struct Point
{
    double x, y;
};

static double
F(double q)
{
    return c*pow(q, 5)+b*pow(q, 3)+a*q;
}

static double
rbc(double r)
{
    r = fabs(r);
    if (r > 1)
	r = 1;
    return F(sqrt(1 - r*r));
}

static int
q2xy(double q, double *x, double *y)
{
    *x = sqrt(1 - q*q);
    *y = F(q);
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
    hi = 1;
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

int
main()
{
    double lo, hi, q, x, y, x0, y0;
    int i, n;
    lo = 0;
    hi = 1;
    n = 100;

    for (i = 0; i < n; i++) {
	q = lo + i*(hi - lo)/(n - 1);
	q2xy(q, &x, &y);
	printf("%g %g\n", x, y);
    }

    min(x = 0.5, y = 0.5, &x0, &y0);
    printf("%g %g\n", x, y);
    printf("%g %g\n", x0, y0);    
}
