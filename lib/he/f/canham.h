#define T HeFCanham

typedef struct He He;
typedef struct T T;

int he_f_canham_ini(real Kb, real C0, real Kad, real DA0D, He*, T**);
int he_f_canham_fin(T*);

int he_f_canham_force(T*, He*,
                      const real *x, const real *y, const real *z, /**/
                      real *fx, real *fy, real *fz);
real he_f_canham_energy(T*, He*, const real *x, const real *y, const real *z);


int he_f_canham_area_ver(T*, /**/ real**);
int he_f_canham_laplace_ver(T*, /**/ real**, real**, real**);
int he_f_canham_norm_ver(T*, /**/ real**, real**, real**);
int he_f_canham_curva_mean_ver(T*, /**/ real**);
int he_f_canham_curva_gauss_ver(T*, /**/ real**);
int he_f_canham_energy_ver(T*, /**/ real**);

#undef T
