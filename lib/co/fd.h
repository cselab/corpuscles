typedef struct He He;
typedef struct Force Force;

/* finite difference */
int fd(Force*, He*, real delta, real *x, real *y, real *z,
       /**/ real *fx, real *fy, real *fz);
