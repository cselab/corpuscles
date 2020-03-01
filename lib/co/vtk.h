typedef struct He He;
enum { VTK_INT, VTK_REAL };
int vtk_write(He *, const real *, const real *, const real *,
              const real **, const char **, /**/ FILE *);
int vtk_tri_write(He *, const real *, const real *, const real *,
                  const real **, const char **, /**/ FILE *);
int vtk_tri_int_write(He *, const real *, const real *, const real *,
                      const int **, const char **, /**/ FILE *);
int vtk_tri_int_read(FILE *, const char *[], /**/ He **, real **, real **,
                     real **, int **[]);
int vtk_grid(FILE *, int size[3], real origin[3], real spacing[3], const real *);
