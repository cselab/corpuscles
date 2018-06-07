#include <stdio.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_odeiv2.h>

#include <he/macro.h>

int func (__UNUSED double t, const double y[], double f[], void *params) {
    double mu;
    mu = *(double*)params;
    f[0] = y[1];
    f[1] = -y[0] - mu*y[1]*(y[0]*y[0] - 1);
    return GSL_SUCCESS;
}

int main (void) {
    double mu = 10;
    gsl_odeiv2_system sys = {func, NULL, 2, &mu};
    gsl_odeiv2_driver * d =
        gsl_odeiv2_driver_alloc_y_new (&sys, gsl_odeiv2_step_rk8pd,
                                       1e-6, 1e-6, 0.0);
    int i;
    double t = 0.0, t1 = 100.0;
    double y[2] = { 1.0, 0.0 };
    double ti;
    int status;

    for (i = 1; i <= 100; i++)
    {
        ti = i * t1 / 100.0;
        status = gsl_odeiv2_driver_apply (d, &t, ti, y);
        if (status != GSL_SUCCESS) {
            printf ("error, return value=%d\n", status);
            break;
     	}
        printf ("%.5e %.5e %.5e\n", t, y[0], y[1]);
    }
    gsl_odeiv2_driver_free (d);
    return 0;
}
