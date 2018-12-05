#define T HeFMeyer

typedef struct He He;
typedef struct T T;

int he_f_meyer_ini(real Kb, He*, T**);
int he_f_meyer_fin(T*);

int he_f_meyer_force(T*, He*,
                      const real *x, const real *y, const real *z, /**/
                      real *fx, real *fy, real *fz);
real he_f_meyer_energy(T*, He*, const real *x, const real *y, const real *z);

int he_f_meyer_area_ver(T*, /**/ real**);
int he_f_meyer_laplace_ver(T*, /**/ real**, real**, real**);
int he_f_meyer_norm_ver(T*, /**/ real**, real**, real**);
int he_f_meyer_curva_mean_ver(T*, /**/ real**);
int he_f_meyer_curva_gauss_ver(T*, /**/ real**);
int he_f_meyer_energy_ver(T*, /**/ real**);
int he_f_meyer_components(T*, /**/ real**, real**, real**, real**);

#undef T
