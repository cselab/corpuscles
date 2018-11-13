typedef struct Ten Ten;

int dtri_angle(const real[3], const real[3], const real[3],  /**/ real[3], real[3], real[3]);
int dtri_area(const real[3], const real[3], const real[3],   /**/ real[3], real[3], real[3]);
int dtri_cot(const real[3], const real[3], const real[3],   /**/ real[3], real[3], real[3]);
int dtri_volume(const real[3], const real[3], const real c[3], /**/ real[3], real[3], real[3]);

int dtri_normal(const real a[3], const real b[3], const real c[3], /**/ Ten *x, Ten *y, Ten *z);
