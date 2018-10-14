#define T HeFJuelicher

typedef struct He He;
typedef struct T T;

int he_f_juelicher_ini(real K, real C0, real Kad, real DA0D, He*, T**);
int he_f_juelicher_fin(T*);

int he_f_juelicher_force(T*, He*,
                      const real *x, const real *y, const real *z, /**/
                      real *fx, real *fy, real *fz);
real he_f_juelicher_energy(T*, He*, const real *x, const real *y, const real *z);

int he_f_juelicher_curva_mean(T*, /**/ real**);
int he_f_juelicher_energy_ver(T*, /**/ real**);
int he_f_juelicher_area_ver(T*, /**/ real**);

#undef T
