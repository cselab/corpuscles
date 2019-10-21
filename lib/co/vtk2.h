#define T Vtk2
typedef struct T T;
int vtk2_ini(const real lo[2], const real hi[2], const int[2], T **);
int vtk2_ascii_ini(const real lo[2], const real hi[2], const int[2], T **);
int vtk2_fin(T *);
int vtk2_fwrite(T *, const real * field[], const char *name[], FILE *);
int vtk2_fwrite1(T *, const real *, const char *, FILE *);
int vtk2_fwrite2(T *, const real *, const real *, const char *,
                 const char *, FILE *);
int vtk2_fwrite3(T *, const real *, const real *, const real *,
                 const char *, const char *, const char *, FILE *);
#undef T
