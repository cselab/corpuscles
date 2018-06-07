#include <stdio.h>
#include <assert.h>

#include <gsl/gsl_errno.h>
#include <gsl/gsl_odeiv2.h>

#include <he/macro.h>

#include "sys.h"

int main (void) {
    Param param;
    gsl_odeiv2_system sys = {f, NULL, DIM, &param};
    gsl_odeiv2_driver *driver;
    int i;
    double t, t1;
    double y[DIM] = { 1.0, 1.0, 1.0 };
    double ti;
    int n, status;
    double istep, rel, abs;

    istep = 1e-6; /* initial step size */
    rel = 1e-6;
    abs = 0.0;
    param.b = 8/3.0; param.s = 10; param.r = 28;
    n = 20000;
    t = 0.0, t1 = 200.0;

    driver = gsl_odeiv2_driver_alloc_y_new (&sys, gsl_odeiv2_step_rk8pd,
                                       istep, rel, abs);
    for (i = 1; i <= n; i++) {
        ti = i * t1 / n;
        status = gsl_odeiv2_driver_apply(driver, &t, ti, y);
        if (status != GSL_SUCCESS) {
            printf ("error, return value=%d\n", status);
            break;
        }
        printf ("%.5e %.5e %.5e %.5e\n",
                t, y[0], y[1], y[2]);
    }
    gsl_odeiv2_driver_free(driver);
    return 0;
}
