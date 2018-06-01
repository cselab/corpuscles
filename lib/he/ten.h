typedef struct Ten { real t[3*3]; } Ten;

void ten_dyadic(real[3], real[3], /**/ Ten*);

void ten_plus(Ten, Ten, /**/ Ten*);
void ten_minus(Ten, Ten, /**/ Ten*);
void ten_scalar(Ten, real, /**/ Ten*);
void ten_vec(Ten, real[3], /**/ real[3]);

/* y += x */
void ten_add(Ten x, /*io*/ Ten *y);

/* y += a*x */
void ten_axpy(real a, Ten x, /*io*/ Ten *y);
void ten_copy(Ten, /**/ Ten*);
void ten_zero(/**/ Ten*);
void ten_one(/**/ Ten*);

int ten_sprintf(Ten, const char *fmt, /**/ char *s);
int ten_fprintf(Ten, FILE*, const char *fmt);
int ten_printf(Ten, const char *fmt);

int ten_fscanf(FILE*, /**/ Ten*);

/* read from argument list */
int ten_argv(const char **pargv[], /**/ Ten*);
