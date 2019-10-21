real edg2_sq(const real[2], const real[2]);
real edg2_abs(const real[2], const real[2]);
real edg2_alpha(const real[2], const real[2], const real[2],
                const real[2]);
real edg2_strain(real Ka, real a3, real a4, const real[2], const real[2],
                 const real[2], const real[2]);
real edg2_point_distance2(const real[2], const real[2], const real p[2]);
real edg2_point_distance(const real[2], const real[2], const real p[2]);
int edg2_point_closest(const real[2], const real[2], const real p[2],
                       real[2]);
int edg2_vect(const real[2], const real[2], FILE *);
