enum {DIM = 3 * 2};
typedef struct Param Param;
struct Param { double b, s, r; };

static Param param;
static double q[DIM];

static void ini(void) {
    enum {X, Y, Z};
    double *q0, *p0;
    param.b = 8/3.0; param.s = 10; param.r = 28;

    q0 = q;
    p0 = q + 3;
    
    q0[X] = q0[Y] = q0[Z] = 1.0;
    p0[X] = 1.0; p0[Y] = 0.0; p0[Z] = 0.0;
}

static int fp(const double q[], const double p[], double f[]) {
    enum {X, Y, Z};
    double b, s, r;
    double x, y, z, xp, yp, zp, dx, dy, dz;
    b = param.b; s = param.s; r = param.r;

    x = q[X]; y = q[Y]; z = q[Z];
    xp = p[X]; yp = p[Y]; zp = p[Z];

    dx = s*(yp - xp);
    dy = (r - z)*xp - yp - x*zp;
    dz = y*xp + x*yp - b*zp;

    f[X] = dx; f[Y] = dy; f[Z] = dz;

    return HE_OK;
}

static int fq(const double q[], double f[]) {
    enum {X, Y, Z};
    double b, s, r;
    double x, y, z, dx, dy, dz;

    b = param.b; s = param.s; r = param.r;
    x = q[X]; y = q[Y]; z = q[Z];

    dx = s*(y-x);
    dy = (-x*z)-y+r*x;
    dz = x*y-b*z;

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


static int print(FILE *f, const char *fmt) {
    int i;
    for (i = 0; i < DIM; i++) {
        if (i != 0) fputc(' ', f);
        if (fprintf(f, fmt, q[i]) < 0)
            ER("failt to print");
    }
    return HE_OK;
}
