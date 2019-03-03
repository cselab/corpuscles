#define T HeFJuelicherXin

typedef struct He He;
typedef struct T T;

int he_f_juelicher_xin_ini(real K, real C0, real Kad, real DA0D, He*, T**);
int he_f_juelicher_xin_argv(char***, He*, T**);
int he_f_juelicher_xin_fin(T*);
int he_f_juelicher_xin_force(T*, He*, const real*, const real*, const real*, /**/ real*, real*, real*);
real he_f_juelicher_xin_energy(T*, He*, const real*, const real*, const real*);
int he_f_juelicher_xin_curva_mean_ver(T*, /**/ real**);
int he_f_juelicher_xin_energy_ver(T*, /**/ real**);
int he_f_juelicher_xin_area_ver(T*, /**/ real**);
real he_f_juelicher_xin_energy_ad(T*);
real he_f_juelicher_xin_energy_bend(T*);

#undef T
