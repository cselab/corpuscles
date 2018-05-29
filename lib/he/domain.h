typedef struct TriDomain TriDomain;

void tri_domain_infinit_ini(TriDomain**);
void tri_domain_periodicX_ini(double xl, double xh, TriDomain**);
void tri_domain_periodicXY_ini(double xl, double xh, double yl, double yh, TriDomain**);
void tri_domain_periodicXYZ_ini(double xl, double xh, double yl, double yh, double zl, double zh, TriDomain**);

void tri_domain_fin(TriDomain*);

/* r = a - b */
void tri_domain_minus(TriDomain*, double ax, double ay, double az, double bx, double by, double bz, /**/
                            double r[3]);
double tri_domain_dist(TriDomain*, double ax, double ay, double az, double bx, double by, double bz);
double tri_domain_sq_dist(TriDomain*, double ax, double ay, double az, double bx, double by, double bz);

void   tri_domain_wrap(TriDomain*, /*io */ double *x, double *y, double *z);
