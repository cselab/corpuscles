#define T HeFMeyerXin

typedef struct He He;
typedef struct T T;

int he_f_meyer_xin_ini(real Kb, real C0, real Kad, real DA0D, He*, T**);
int he_f_meyer_xin_argv(const char***, He*, T**);
int he_f_meyer_xin_fin(T*);
int he_f_meyer_xin_force(T*, He*, const real*, const real*, const real*, /**/ real*, real*, real*);
real he_f_meyer_xin_energy(T*, He*, const real*, const real*, const real*);
int he_f_meyer_xin_area_ver(T*, /**/ real**);
int he_f_meyer_xin_laplace_ver(T*, /**/ real**, real**, real**);
int he_f_meyer_xin_norm_ver(T*, /**/ real**, real**, real**);
int he_f_meyer_xin_curva_mean_ver(T*, /**/ real**);
int he_f_meyer_xin_curva_gauss_ver(T*, /**/ real**);
int he_f_meyer_xin_energy_ver(T*, /**/ real**);
real he_f_meyer_xin_energy_ad(T*);
real he_f_meyer_xin_energy_bend(T*);

#undef T
