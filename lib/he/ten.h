typedef struct Ten { real t[3*3]; } Ten;

void ten_dyadic(const real[3], const real[3], /**/ Ten*);

void ten_plus(const Ten*, const Ten*, /**/ Ten*);
void ten_minus(const Ten*, const Ten*, /**/ Ten*);
void ten_scalar(const Ten*, real, /**/ Ten*);
void ten_vec(const Ten*, const real[3], /**/ real[3]);

/* y += x */
void ten_add(const Ten *x, /*io*/ Ten *y);

/* y += a*x */
void ten_axpy(real a, const Ten *x, /*io*/ Ten *y);
void ten_copy(const Ten*, /**/ Ten*);
void ten_zero(/**/ Ten*);
void ten_one(/**/ Ten*);

int ten_sprintf(const Ten*, const char *fmt, /**/ char *s);
int ten_fprintf(const Ten*, FILE*, const char *fmt);
int ten_printf(const Ten*, const char *fmt);

int ten_fscanf(FILE*, /**/ Ten*);

/* read from argument list */
int ten_argv(const char **pq[], /**/ Ten*);
