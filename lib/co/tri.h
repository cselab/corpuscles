real tri_angle(const real[3], const real b[3], const real[3]);
real tri_area(const real[3], const real[3], const real[3]);
real tri_cot(const real[3], const real b[3], const real[3]);
int tri_edg(const real[3], const real[3], const real[3], /**/ real[3],
            real[3], real[3]);
real tri_volume(const real[3], const real[3], const real[3]);
int tri_normal(const real[3], const real[3], const real[3], /**/ real[3]);
int tri_center(const real[3], const real[3], const real[3], /**/ real[3]);

/* geomveiw objects */
int tri_off(const real[3], const real[3], const real[3], FILE *);
int tri_vect(const real[3], const real[3], const real[3], const real[3],
             const real[3], const real[3], FILE *);
int tri_list(const real[3], const real[3], const real[3], const real[3],
             const real[3], const real[3], FILE *);

/* project triangle to 2D:  a->[0, 0],  b->[ux, 0],  c->[wx, uy] */
int tri_3to2(const real a[3], const real b[3], const real c[3],
             /**/ real * ux, real * wx, real * wy);

/* ex[3], ey[3]: basis local to a triangle
   ex = norm(b - a)
   ey is normal to ex in a plane of a triangle */
int tri_2to3(const real a[3], const real b[3], const real c[3],
             /**/ real ex[3], real ey[3]);

int tri_3d_invariants(const real[3], const real[3], const real[3],
                      const real[3], const real[3], const real[3],
                      /**/ real * al, real * be);
int tri_2d_invariants(real bx, real cx, real cy, real ux, real wx, real wy,
                      /**/ real * al, real * be);

/* |a - b|^2/area */
real tri_edg_area(const real a[3], const real b[3], const real[3]);

real tri_alpha(const real a[3], const real b[3], const real c[3],
               const real u[3], const real v[3], const real w[3]);
real tri_beta(const real a[3], const real b[3], const real c[3],
              const real u[3], const real v[3], const real w[3]);

real tri_lim_area(real Ka, real a3, real a4, const real[3], const real[3],
                  const real[3], const real[3], const real[3],
                  const real[3]);
real tri_lim_shear(real mu, real b1, real b2, const real[3], const real[3],
                   const real[3], const real[3], const real[3],
                   const real[3]);
real tri_lim(real Ka, real a3, real a4, real mu, real b1, real b2,
             const real[3], const real[3], const real[3], const real[3],
             const real[3], const real[3]);

int tri_abc(const real[3], const real[3], const real[3], const real[3],
            const real[3], const real[3], /**/ real *, real *, real *);

real tri_point_distance2(const real[3], const real[3], const real[3],
                         const real[3]);
real tri_point_distance(const real[3], const real[3], const real[3],
                        const real[3]);
int tri_point_closest(const real[3], const real[3], const real[3],
                      const real[3], /**/ real[3]);
int tri_moment(const real[3], const real[3], const real[3], /**/ real *,
               real *, real *, real *, real *, real *);
/* center of opposing edge */
int tri_edg_center(const real[3], const real[3], const real[3],
                   /**/ real[3], real[3], real[3]);
