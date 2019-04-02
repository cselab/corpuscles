int predicate_ini(void);
real predicate_orient2d(const real[2], const real[2], const real[2]);
real predicate_orient3d(const real[3], const real[3], const real[3], const real[3]);
real predicate_incircle(const real[2], const real[2], const real[2], const real[2]);
real predicate_insphere(const real[3], const real[3], const real[3], const real[3], const real[3]);

/* does a ray de crosses a triangl abc? */
int predicate_ray(const real d[3], const real e[3], const real a[3], const real b[3], const real c[3]);
int predicate_orient3d_sas(const real[3], const real[3], const real[3], const real[3]);
