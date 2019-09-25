#include <stdio.h>

int
main()
{
    double *pre;
    double **pot, grad, sigma_dl, sigma_dv, sigma_sl, source;
    int *ier;
    int iprec, ifsingle, ifdouble, ifpot, ifgrad, nparts;
    stfmm3_dpartself(ier,iprec,nparts,source,
		     ifsingle,sigma_sl,ifdouble,sigma_dl,sigma_dv,
		     ifpot,pot,pre,ifgrad,grad);
}
