#define T HeFAreaVoronoi

typedef struct He He;
typedef struct T T;

int he_f_area_voronoi_ini(real Kb, real C0, real Kad, real DA0D, He*, T**);
int he_f_area_voronoi_fin(T*);
int he_f_area_voronoi_force(T*, He*,
                      const real *x, const real *y, const real *z, /**/
                      real *fx, real *fy, real *fz);
real he_f_area_voronoi_energy(T*, He*, const real *x, const real *y, const real *z);

#undef T
