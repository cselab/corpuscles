#define T HeFGareaVoronoi

typedef struct He He;
typedef struct T T;

int he_f_garea_voronoi_ini(real a0, real K, He*, T**);
int he_f_garea_voronoi_fin(T*);
int he_f_garea_voronoi_force(T*, He*, const real *x, const real *y, const real *z, /**/ real *fx, real *fy, real *fz);
real he_f_garea_voronoi_energy(T*, He*, const real *x, const real *y, const real *z);

#undef T
