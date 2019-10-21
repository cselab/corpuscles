typedef struct He He;
typedef struct Force Force;

/* finite difference */
int fd(Force *, He *, real delta, real *, real *, real *, /**/ real *,
       real *, real *);
