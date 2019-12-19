typedef struct He He;
enum { VTK_INT, VTK_REAL };
int vtk_write(He *, const real *, const real *, const real *,
               const real **, const char **, /**/ FILE *);
int vtk_tri_write(He *, const real *, const real *, const real *,
                   const real **, const char **, /**/ FILE *);
int vtk_tri_int_write(He *, const real *, const real *, const real *,
		       const int **, const char **, /**/ FILE *);
