int vec_ini(real, real, real, /**/ real[3]);
int vec_get(int, const real[], const real[], const real[], /**/ real[3]);
int vec_set(real[3], int, /**/ real[], real[], real[]);

int vec_coord(real[3], real*, real*, real*);

int vec_plus(real[3], real[3], /**/ real[3]);
int vec_minus(real[3], real[3], /**/ real[3]);

 /* al*a + be*b */
int vec_linear_combination(real, real[3], real, real[3], /**/ real[3]);

int vec_scalar(real[3], real, /**/ real[3]);
int vec_negative(real[3], /**/ real[3]);

int vec_append(real[3],  int, /**/ real*, real*, real*);
int vec_substr(real [3], int, /**/ real*, real*, real*);
int vec_scalar_append(real[3], real, int, /**/ real*, real*, real*);

real vec_dot(real[3], real[3]);
real vec_angle(real[3], real[3]);

real vec_abs(real[3]);
int vec_cross(real[3], real[3], /**/ real[3]);

/* coordinate system */
real vec_cylindrical_r(real[3]);
real vec_spherical_theta(real[3]);

int vec_norm(real[3], /**/ real[3]);
int vec_fprintf(real a[3], FILE *f, const char *fmt);
int vec_printf(real a[3], const char *fmt);

int vec_argv(const char **pq[], /**/ real[3]);
int vec_zero(/**/ real[3]);
