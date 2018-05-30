void vec_ini(real x, real y, real z, /**/ real[3]);
void vec_coord(real[3], real *x, real *y, real *z);

void vec_plus(real[3], real[3], /**/ real[3]);
void vec_minus(real[3], real[3], /**/ real[3]);

void vec_scalar(real[3], real, /**/ real[3]);
void vec_append(real[3], /**/ real *x, real *y, real *z);
void vec_append_direction(real[3], real s, /**/ real *x, real *y, real *z);

real vec_dot(real[3], real[3]);
real vec_abs(real[3]);
void vec_cross(real[3], real[3], /**/ real[3]);

void vec_norm(real[3], /**/ real[3]);
int vec_printf(real a[3], FILE *f, const char *fmt);
