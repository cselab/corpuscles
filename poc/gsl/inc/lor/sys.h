enum {DIM = 3};
typedef struct Param Param;
struct Param { double b, s, r; };

int f(__UNUSED double t, const double q[], double f[], void *vparam) {
    Param *param;
    double b, s, r;
    double x, y, z, dx, dy, dz;
    assert(DIM == 3);

    param = (Param*)vparam;
    b = param->b; s = param->s; r = param->r;
    x = q[0]; y = q[1]; z = q[2];

    dx = s*(y-x);
    dy = (-x*z)-y+r*x;
    dz = x*y-b*z;

    f[0] = dx; f[1] = dy; f[2] = dz;

    return GSL_SUCCESS;
}

