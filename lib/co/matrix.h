int matrix_zero(int, int, real*);

int matrix_mult_nn(int, int, int, const real*, const real*, /**/ real*);
int matrix_mult_nt(int, int, int, const real*, const real*, /**/ real*);
int matrix_mult_tn(int, int, int, const real*, const real*, /**/ real*);
int matrix_mult_tt(int, int, int, const real*, const real*, /**/ real*);

int matrix_fwrite(int, int, const real*, FILE*);
int matrix_fread(FILE*, /**/ int*, int*, real**);
int matrix_ini(int, int, real**);
int matrix_free(real*);
