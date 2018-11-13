typedef struct Ten { real t[3*3]; } Ten;

int ten_col_ini(const real[3], const real[3], const real[3], /**/ Ten*);
int ten_row_ini(const real[3], const real[3], const real[3], /**/ Ten*);
int ten_dyadic(const real[3], const real[3], /**/ Ten*);

int ten_plus(const Ten*, const Ten*, /**/ Ten*);
int ten_minus(const Ten*, const Ten*, /**/ Ten*);
int ten_scalar(const Ten*, real, /**/ Ten*);
int ten_vec(const Ten*, const real[3], /**/ real[3]);

/* y += x */
int ten_add(const Ten *x, /*io*/ Ten *y);
real ten_trace(const Ten*);
/* y += a*x */
int ten_axpy(real a, const Ten *x, /*io*/ Ten *y);
int ten_copy(const Ten*, /**/ Ten*);
int ten_zero(/**/ Ten*);
int ten_one(/**/ Ten*);

int ten_sprintf(const Ten*, const char *fmt, /**/ char *s);
int ten_fprintf(const Ten*, FILE*, const char *fmt);
int ten_printf(const Ten*, const char *fmt);

int ten_fscanf(FILE*, /**/ Ten*);
int ten_line(const Ten *T);

/* read from argument list */
int ten_argv(const char **pq[], /**/ Ten*);
