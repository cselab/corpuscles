#include <stdio.h>
#include <assert.h>
#include <math.h>

#include <gsl/gsl_errno.h>
#include <gsl/gsl_odeiv2.h>

#include <he/err.h>
#include <he/macro.h>

#include "sys.h"

int main(void) {
    gsl_odeiv2_system sys = {f, NULL, DIM, &param};
    gsl_odeiv2_driver *driver;
    int i;
    double t, t1;
    double ti;
    int n, status;
    double istep, rel, abs;

    istep = 1e-6; /* initial step size */
    rel = 1e-14;
    abs = 0.0;
    n = 10000;
    t = 0.0, t1 = 1200.0;
    ini();
    
    driver = gsl_odeiv2_driver_alloc_y_new (&sys, gsl_odeiv2_step_rkck,
                                       istep, rel, abs);
    for (i = 1; i <= n; i++) {
        ti = i * t1 / n;
        status = gsl_odeiv2_driver_apply(driver, &t, ti, q);
        if (status != GSL_SUCCESS) {
            printf ("error, return value=%d\n", status);
            break;
        }
        //norm3(/**/ q + 3);
        printf("%.5e ", t);
        print(stdout, "%.5e");
        putc('\n', stdout);
    }
    gsl_odeiv2_driver_free(driver);
    return 0;
}
