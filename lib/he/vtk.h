typedef struct He He;
int vtk_fwrite(He *he, const real *x, const real *y, const real *z, const real *scalars[], const char *names[], /**/
                  FILE *f);
int vtk_write(He *he, const real *x, const real *y, const real *z, const real *scalars[], const char *names[], /**/
                 const char *path);
