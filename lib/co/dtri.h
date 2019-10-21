typedef struct Ten Ten;
int dtri_angle(const real[3], const real[3], const real[3], /**/ real[3],
               real[3], real[3]);
int dtri_area(const real[3], const real[3], const real[3], /**/ real[3],
              real[3], real[3]);
int dtri_cot(const real[3], const real[3], const real[3], /**/ real[3],
             real[3], real[3]);
int dtri_volume(const real[3], const real[3], const real c[3],
                /**/ real[3], real[3], real[3]);
int dtri_normal(const real a[3], const real b[3], const real c[3],
                /**/ Ten * x, Ten * y, Ten * z);
/* |a - b|^2/area */
int dtri_edg_area(const real a[3], const real b[3], const real[3],
                  /**/ real[3], real[3], real[3]);
int dtri_alpha(const real[3], const real[3], const real[3],
               const real u[3], const real v[3], const real w[3],
               /**/ real[3], real[3], real[3]);
int dtri_beta(const real[3], const real[3], const real[3], const real u[3],
              const real v[3], const real w[3], /**/ real[3], real[3],
              real[3]);
int dtri_lim_area(real Ka, real a3, real a4, const real[3], const real[3],
                  const real[3], const real[3], const real[3],
                  const real[3], /**/ real[3], real[3], real[3]);
int dtri_lim_shear(real mu, real b1, real b2, const real[3], const real[3],
                   const real[3], const real[3], const real[3],
                   const real[3], /**/ real[3], real[3], real[3]);
int dtri_lim(real Ka, real a3, real a4, real mu, real b1, real b2,
             const real[3], const real[3], const real[3], const real[3],
             const real[3], const real[3], /**/ real[3], real[3], real[3]);
