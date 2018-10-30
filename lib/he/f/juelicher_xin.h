#define T HeFJuelicherXin

typedef struct He He;
typedef struct T T;

int he_f_juelicher_xin_ini(real K, real C0, real Kad, real DA0D, He*, T**);
int he_f_juelicher_xin_fin(T*);

int he_f_juelicher_xin_force(T*, He*,
                      const real *x, const real *y, const real *z, /**/
                      real *fx, real *fy, real *fz);
real he_f_juelicher_xin_energy(T*, He*, const real *x, const real *y, const real *z);

int he_f_juelicher_xin_curva_mean_ver(T*, /**/ real**);
int he_f_juelicher_xin_energy_ver(T*, /**/ real**);
int he_f_juelicher_xin_area_ver(T*, /**/ real**);

#undef T
