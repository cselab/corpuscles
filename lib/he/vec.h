void vec_ini(double x, double y, double z, /**/ double[3]);
void vec_coord(double[3], double *x, double *y, double *z);

void vec_plus(double[3], double[3], /**/ double[3]);
void vec_minus(double[3], double[3], /**/ double[3]);

void vec_scalar(double[3], double, /**/ double[3]);
void vec_append(double[3], /**/ double *x, double *y, double *z);
void vec_append_direction(double[3], double s, /**/ double *x, double *y, double *z);

double vec_dot(double[3], double[3]);
double vec_abs(double[3]);
void vec_cross(double[3], double[3], /**/ double[3]);

void vec_norm(double[3], /**/ double[3]);
int vec_printf(double a[3], FILE *f, const char *fmt);
