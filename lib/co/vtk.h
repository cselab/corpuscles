typedef struct He He;
int vtk_fwrite(He *, const real *, const real *, const real *,
               const real **, const char **, /**/ FILE *);
int vtk_write(He *, const real *, const real *, const real *,
              const real **, const char **, /**/ const char *);
int vtk_tri_fwrite(He *, const real *, const real *, const real *,
                   const real **, const char **, /**/ FILE *);
int vtk_tri_write(He *, const real *, const real *, const real *,
                  const real **, const char **, /**/ const char *);
