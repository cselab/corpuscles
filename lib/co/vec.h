int vec_ini(real, real, real, /**/ real[3]);
int vec_get(int, const real[], const real[], const real[], /**/ real[3]);
int vec_get3(int, int, int, const real[], const real[], const real[], /**/ real[3], real[3], real[3]);
int vec_set(const real[3], int, /**/ real[], real[], real[]);

int vec_coord(const real[3], /**/ real*, real*, real*);

int vec_plus(const real[3], const real[3], /**/ real[3]);
int vec_minus(const real[3], const real[3], /**/ real[3]);

int vec_add(const real[3], /*io*/ real[3]);
int vec_sub(const real[3], /*io*/ real[3]);
/* y += a*x */
int vec_axpy(real a, const real x[3], /*io*/ real y[3]);
/* x = -x */
int vec_neg(/*io*/ real[3]);

 /* al*a + be*b */
int vec_linear_combination(real, const real[3], real, const real[3], /**/ real[3]);

int vec_scalar(const real[3], real, /**/ real[3]);
int vec_negative(const real[3], /**/ real[3]);
int vec_copy(const real[3], /**/ real[3]);
int vec_scale(real, /*io*/ real[3]);

int vec_append(const real[3],  int, /**/ real*, real*, real*);
int vec_substr(const real [3], int, /**/ real*, real*, real*);
int vec_scalar_append(const real[3], real, int, /**/ real*, real*, real*);

real vec_dot(const real[3], const real[3]);
real vec_angle(const real[3], const real[3]);

real vec_abs(const real[3]);
int vec_cross(const real[3], const real[3], /**/ real[3]);

/* coordinate system */
real vec_cylindrical_r(const real[3]);
real vec_spherical_theta(const real[3]);

int vec_norm(const real[3], /**/ real[3]);
int vec_normalize(real[3]);
int vec_mean3(const real[3], const real[3], const real[3], /**/ real[3]);

/* `c' = project `a' on `b' */
int vec_project(const real[3], const real[3], /**/ real[3]);
real vec_project_scalar(const real[3], const real[3]);

/* `c' = reject`a' on `b' */
int vec_reject(const real[3], const real[3], /**/ real[3]);
real vec_reject_scalar(const real[3], const real[3]);

int vec_fprintf(const real[3], FILE*, const char *fmt);
int vec_fprintf0(const real[3], FILE*, const char *fmt);
int vec_printf(const real[3], const char *fmt);

int vec_argv(const char **pq[], /**/ real[3]);
int vec_zero(/**/ real[3]);
