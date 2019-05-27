#define T Vtk2
typedef struct T T;
vtk2_ini(int n, const real lo[2], const real hi[2], const int[2], T**);
vtk2_fin(T**);
vtk2_fwrite(const *real[], const *char[], FILE*);
#undef T