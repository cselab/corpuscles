typedef struct He He;
enum {VTK_INT, VTK_REAL};
int vtk_fwrite(He *, const real *, const real *, const real *,
               const real **, const char **, /**/ FILE *);
int vtk_tri_fwrite(He *, const real *, const real *, const real *,
                   const real **, const char **, /**/ FILE *);
