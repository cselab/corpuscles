#include <stdio.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_odeiv2.h>

#include <he/macro.h>

typedef struct Param Param;
struct Param { double b, s, r; };

int func (__UNUSED double t, const double q[], double f[], void *vparam) {
    Param *param;
    double b, s, r;
    double x, y, z, dx, dy, dz;

    param = (Param*)vparam;
    b = param->b; s = param->s; r = param->r;
    x = q[0]; y = q[1]; z = q[2];

    dx = s*(y-x);
    dy = (-x*z)-y+r*x;
    dz = x*y-b*z;

    f[0] = dx; f[1] = dy; f[2] = dz;

    return GSL_SUCCESS;
}

int main (void) {
    Param param;
    gsl_odeiv2_system sys = {func, NULL, 2, &param};
    gsl_odeiv2_driver *driver;
    int i;
    double t = 0.0, t1 = 100.0;
    double y[2] = { 1.0, 0.0 };
    double ti;
    int status;
    double istep, rel, abs;

    istep = 1e-6; /* initial step size */
    rel = 1e-6;
    abs = 0.0;
    param.b = 8.0/3; param.s = 10; param.r = 28;

    driver = gsl_odeiv2_driver_alloc_y_new (&sys, gsl_odeiv2_step_rk8pd,
                                       istep, rel, abs);
    for (i = 1; i <= 100; i++)
    {
        ti = i * t1 / 100.0;
        status = gsl_odeiv2_driver_apply(driver, &t, ti, y);
        if (status != GSL_SUCCESS) {
            printf ("error, return value=%d\n", status);
            break;
        }
        printf ("%.5e %.5e %.5e\n", t, y[0], y[1]);
    }
    gsl_odeiv2_driver_free(driver);
    return 0;
}
