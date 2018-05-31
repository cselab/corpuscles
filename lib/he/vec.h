void vec_ini(real, real, real, /**/ real[3]);
void vec_get(int, real[], real[], real[], /**/ real[3]);
void vec_set(int, real[], real[], real[], /**/ real[3]);

void vec_coord(real[3], real*, real*, real*);

void vec_plus(real[3], real[3], /**/ real[3]);
void vec_minus(real[3], real[3], /**/ real[3]);

void vec_scalar(real[3], real, /**/ real[3]);
void vec_append(real[3], /**/ real*, real*, real*);

real vec_dot(real[3], real[3]);
real vec_angle(real[3], real[3]);

real vec_abs(real[3]);
void vec_cross(real[3], real[3], /**/ real[3]);

/* coordinate system */
real vec_cylindrical_r(real[3]);

void vec_norm(real[3], /**/ real[3]);
int vec_printf(real a[3], FILE *f, const char *fmt);
