void stfmm3_dpartself(int *ier, int iprec, int nparts, const double *source,
		      int ifsingle, const double *sigma_sl,
		      int ifdouble, const double *sigma_dl, const double *sigma_dv,
		      int ifvel, double *vel, double *pre, int ifgrad, double *grad);

void stfmm3_dparttarg(int *ier, int iprec, int nparts, const double *source,
		      int ifsingle, const double *sigma_sl,
		      int ifdouble, const double *sigma_dl, const double *sigma_dv,
		      int ifvel, double *vel, double *pre, int ifgrad, double *grad,
		      int ntargs, const double *target,
		      int ifpottarg, double *pottarg, double *pretarg, int ifgradtarg, double *gradtarg);

void stfmm3_dpartdirect(int nparts, const double *source,
			int ifsingle, const double *sigma_sl,
			int ifdouble, const double *sigma_dl, const double *sigma_dv,
			int ifvel, double *vel, double *pre, int ifgrad, double *grad,
			int ntargs, const double *target,
			int ifpottarg, double *pottarg, double *pretarg, int ifgradtarg, double *gradtarg);
