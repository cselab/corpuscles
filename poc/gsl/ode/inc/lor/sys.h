enum {DIM = 3};
typedef struct Param Param;
struct Param { double b, s, r; };

static Param param;
static double q[DIM];

static void ini(void) {
    enum {X, Y, Z};
    double a[DIM] = {-4.79874e+00, -3.87759e+00, 2.42234e+01};
    double b[DIM] = {-4.84818e+00, -5.81392e+00, 2.08459e+01};
    param.b = 8/3.0; param.s = 10; param.r = 28;
    // q[0] = q[1] = q[2] = 1.0;
    
    q[X] = (a[X] + b[X])/2;
    q[Y] = (a[Y] + b[Y])/2;
    q[Z] = (a[Z] + b[Z])/2;
}

static int f(__UNUSED double t, const double q[], double f[], __UNUSED void *vparam) {
    double b, s, r;
    double x, y, z, dx, dy, dz;
    assert(DIM == 3);

    b = param.b; s = param.s; r = param.r;
    x = q[0]; y = q[1]; z = q[2];

    dx = s*(y-x);
    dy = (-x*z)-y+r*x;
    dz = x*y-b*z;

    f[0] = dx; f[1] = dy; f[2] = dz;

    return GSL_SUCCESS;
}

static int print(FILE *f, const char *fmt) {
    int i;
    for (i = 0; i < DIM; i++) {
        if (i != 0) fputc(' ', f);
        if (fprintf(f, fmt, q[i]) < 0)
            ER("failt to print");
    }
    return CO_OK;
}
