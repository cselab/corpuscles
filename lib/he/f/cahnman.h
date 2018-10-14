#define T HeFCahnman

typedef struct He He;
typedef struct T T;

int he_f_canham_ini(real K, He*, T**);
int he_f_canham_fin(T*);

int he_f_canham_force(T*, He*,
                      const real *x, const real *y, const real *z, /**/
                      real *fx, real *fy, real *fz);
real he_f_canham_energy(T*, He*, const real *x, const real *y, const real *z);

int he_f_canham_curva_mean(T*, /**/ real**);
int he_f_canham_energy_ver(T*, /**/ real**);
int he_f_canham_area_ver(T*, /**/ real**);

#undef T
