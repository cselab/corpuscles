int vec2_ini(real, real, /**/ real[2]);
int vec2_get(int, const real[], const real[], /**/ real[2]);

int vec2_add(const real[2], /*io*/ real[2]);
int vec2_copy(const real[2], real[2]);
int vec2_cov(const real[3], real[2]);
int vec2_minus(const real[2], const real[2], /**/ real[2]);
int vec2_negative(const real[2], real[2]);
int vec2_normalize(real[2]);
int vec2_scalar_append(const real[2], real, int, real*, real*);
int vec2_scale(real, real a[2]);
int vec2_sub(const real[2], /*io*/ real[2]);
real vec2_abs(const real[2]);
real vec2_angle(const real[2], const real[2]);
real vec2_cross(const real[2], const real[3]);
real vec2_dot(const real[2], const real[2]);

int vec2_argv(const char **pq[], /**/ real[2]);
int vec2_fprintf(const real[2], FILE *f);