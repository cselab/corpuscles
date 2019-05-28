#define T Vtk2
typedef struct T T;
int vtk2_ini(const real lo[2], const real hi[2], const int[2], T**);
int vtk2_fin(T*);
int vtk2_fwrite(T*, const real *field[], const char *name[], FILE*);
#undef T