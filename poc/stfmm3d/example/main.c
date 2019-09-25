#include <stdio.h>
#include <stdlib.h>
#include <stfmm3.h>

int
main()
{
    int i, j;
    double *pre;
    double *vel, *grad, *sigma_dl, *sigma_dv, *sigma_sl, *source;
    int ier;
    int iprec, ifsingle, ifdouble, ifvel, ifgrad, nparts;

    iprec = 3; /* < 0.5e-9 */
    nparts = 4;
    source = malloc(3*nparts*sizeof(*source));
    sigma_sl = malloc(3*nparts*sizeof(*sigma_sl));
    vel = malloc(3*nparts*sizeof(*sigma_sl));
    pre = malloc(3*nparts*sizeof(*sigma_sl));
    grad = malloc(3*3*nparts*sizeof(*sigma_sl));

    ifsingle = 1;
    ifdouble = 0;
    sigma_dl = sigma_dv = NULL;
    ifvel = 1;
    ifgrad = 1;

    for (i = j = 0; i < nparts; i++) {
	source[j++] = 1 * (i + 1);
	source[j++] = 2 * (i + 1);
	source[j++] = 3 * (i + 1);
    }

    for (i = j = 0; i < nparts; i++) {
	sigma_sl[j++] = 1 * (i + 1);
	sigma_sl[j++] = 2 * (i + 1);
	sigma_sl[j++] = 3 * (i + 1);
    }

    stfmm3_dpartself(&ier, iprec, nparts, source,
		     ifsingle, sigma_sl, ifdouble, sigma_dl, sigma_dv,
		     ifvel, vel, pre, ifgrad, grad);

    for (i = j = 0; i < nparts; i++) {
	printf("%g ", vel[j++]);
	printf("%g ", vel[j++]);
	printf("%g\n", vel[j++]);
    }

    free(source);
    free(sigma_sl);
    free(vel);
    free(pre);
    free(grad);
}
