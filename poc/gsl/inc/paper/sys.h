enum {DIM = 3 * 2};
typedef struct Param Param;
struct Param { double al, om, la, be;};

static Param param;
static double q[DIM];

static void norm3(double a[3]) {
    enum {X, Y, Z};
    double r, eps;
    eps = 1e-20;
    r = a[X]*a[X] + a[Y]*a[Y] + a[Z]*a[Z];
    if (r < eps) return;
    r = sqrt(r);
    a[X] /= r; a[Y] /= r; a[Z] /= r;
}

static int fp(const double q[], const double p[], double f[]) {
    enum {X, Y, Z};
    double al, om, la, be;
    double x, y, z, xp, yp, zp, dx, dy, dz;
    al = param.al; om = param.om; la = param.la; be = param.be;

    x = q[X]; y = q[Y]; z = q[Z];
    xp = p[X]; yp = p[Y]; zp = p[Z];

    dx = 2*z*zp+(2*om*x+om)*yp+xp*(2*om*y+2*al*x+al);
    dy = (2*al*x+al)*yp+xp*(2*al*y-2*om*x-om);
    dz = ((-(la+be)*x)-la)*zp-(la+be)*xp*z;

    f[X] = dx; f[Y] = dy; f[Z] = dz;

    return HE_OK;
}

static double sq(double x) { return x*x; }
static int fq(const double q[], double f[]) {
    enum {X, Y, Z};
    double al, om, la, be;
    double x, y, z, dx, dy, dz;

    al = param.al; om = param.om; la = param.la; be = param.be;
    x = q[X]; y = q[Y]; z = q[Z];

    dx = al*x + om*y + al*sq(x) + 2*om*x*y + sq(z);
    dy = -om*x + al*y - om*sq(x) + 2*al*x*y;
    dz = -la*z - (la + be)*x*z;

    f[X] = dx; f[Y] = dy; f[Z] = dz;

    return HE_OK;
}

static int f(__UNUSED double t, const double Q[], double dQ[], __UNUSED void *vparam) {
    enum {X, Y, Z};
    const double *p, *q;
    double *dq, *dp;

    q = Q;
    p = Q + 3;

    dq = dQ;
    dp = dQ + 3;

    fq(q,    /**/ dq);
    fp(q, p, /**/ dp);

    return GSL_SUCCESS;
}

static void ini(void) {
    enum {X, Y, Z};
    double *q0, *p0;
    double pi = 3.141592653589793;
    param.al = 0.01;
    param.om = 2*pi;
    param.la = 0.1;
    param.be = 0.1;

    q0 = q;
    p0 = q + 3;

    q0[X] = 0.0; q0[Y] = q0[Z] = 0.01;
    p0[X] = p0[Y] = 1; p0[Z] = 0;

    fq(q0, p0);
    norm3(p0);
}

static int print(FILE *f, const char *fmt) {
    enum {X, Y, Z};
    double v[3];
    int i;
    for (i = 0; i < DIM; i++) {
        if (i != 0) fputc(' ', f);
        if (fprintf(f, fmt, q[i]) < 0)
            ER("failt to print");
    }

    fq(q, /**/ v);
    norm3(/**/ v);

    fprintf(f, " %.16e %.16e %.16e", v[X], v[Y], v[Z]);
    return HE_OK;
}
