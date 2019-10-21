int matrix_zero(int, int, real *);
int matrix_one(int, int, real *);
int matrix_scale(int, int, real, real *);
int matrix_set(int M, int N, int m, int n, real, real *);
int matrix_add(int M, int N, int m, int n, real, real *);
real matrix_get(int M, int N, int m, int n, const real *);
int matrix_mult_nn(int, int, int, const real *, const real *, /**/ real *);
int matrix_mult_nt(int, int, int, const real *, const real *, /**/ real *);
int matrix_mult_tn(int, int, int, const real *, const real *, /**/ real *);
int matrix_mult_tt(int, int, int, const real *, const real *, /**/ real *);
int matrix_array_n(int, int, const real *, const real *, /**/ real *);
int matrix_array_t(int, int, const real *, const real *, /**/ real *);
int matrix_array_append_n(int, int, const real *, const real *,
                          /**/ real *);
int matrix_array_append_t(int, int, const real *, const real *,
                          /**/ real *);
int matrix_array_substr_n(int, int, const real *, const real *,
                          /**/ real *);
int matrix_array_substr_t(int, int, const real *, const real *,
                          /**/ real *);
int matrix_transpose(int, int, /*io */ real *);

int matrix_fwrite(int, int, const real *, FILE *);
int matrix_fread(FILE *, /**/ int *, int *, real **);
int matrix_ini(int, int, real **);
int matrix_fin(real *);
