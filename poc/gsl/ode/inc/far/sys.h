enum {DIM = 3};
typedef struct Param Param;
struct Param { double al, om, la, be; };

static Param param;
static double q[DIM];

static void ini(void) {
    double pi = 3.141592653589793;
    param.al = 0.01;
    param.om = 2*pi;
    param.la = 0.1;
    param.be = 0.1;
    
    q[0] = 0.0; q[1] = q[2] = 0.01;
}


static double sq(double x) { return x*x; }
static int f(__UNUSED double t, const double q[], double f[], __UNUSED void *vparam) {
    double al, om, la, be;
    double x, y, z, dx, dy, dz;
    assert(DIM == 3);

    al = param.al; om = param.om; la = param.la; be = param.be;
    
    x = q[0]; y = q[1]; z = q[2];

    dx = al*x + om*y + al*sq(x) + 2*om*x*y + sq(z);
    dy = -om*x + al*y - om*sq(x) + 2*al*x*y;
    dz = -la*z - (la + be)*x*z;

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
