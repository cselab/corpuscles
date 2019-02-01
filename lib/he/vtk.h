typedef struct He He;
int vtk_fwrite(He*, const real*, const real*, const real*, const real *scalars[], const char *names[], /**/ FILE*);
int vtk_write(He*, const real*, const real*, const real*, const real *scalars[], const char *names[], /**/ const char*);
