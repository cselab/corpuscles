real edg_abs(const real[3], const real[3]);
real edg_sq(const real[3], const real[3]);
real edg_point_distance2(const real[3], const real[3], const real p[3]);
real edg_point_distance(const real[3], const real[3], const real p[3]);
int edg_point_closest(const real[3], const real[3], const real p[3],
                      real q[3]);
int edg_vect(const real[3], const real[3], FILE *);
int edg_center(const real[3], const real[3], /**/ real[3]);
