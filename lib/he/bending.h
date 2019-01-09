#define T Bending

typedef struct He He;
typedef struct T T;

struct BendingParam { real Kb, C0, Kad, DA0D; };
typedef struct BendingParam BendingParam;

int bending_ini(const char *name, BendingParam, He*, /**/ T**);
int bending_fin(T*);

int bending_force(T*, He*, const real *x, const real *y, const real *z, /**/ real *fx, real *fy, real *fz);
real bending_energy(T*, He*, const real *x, const real *y, const real *z);
int bending_energy_ver(T*, /**/ real**);
int bending_area_ver(T*, /**/ real**);
int bending_curva_mean_ver(T*, /**/ real**);
int bending_norm_ver(T*, /**/ real**, real**, real**);
int bending_laplace_ver(T*, /**/ real**, real**, real**);
real bending_energy_bend(T*);
real bending_energy_ad(T*);

const char* bending_list();

int bending_kantor_ini(BendingParam, He*, /**/ T**);
int bending_gompper_ini(BendingParam, He*, /**/ T**);
int bending_gompper_kroll_ini(BendingParam, He*, /**/ T**);
int bending_juelicher_ini(BendingParam, He*, /**/ T**);
int bending_juelicher_xin_ini(BendingParam, He*, /**/ T**);
int bending_meyer_ini(BendingParam, He*, /**/ T**);
int bending_canham_ini(BendingParam, He*, /**/ T**);
int bending_gompper_xin_ini(BendingParam, He*, /**/ T**);
int bending_meyer_xin_ini(BendingParam, He*, /**/ T**);
int bending_meyer_ini(BendingParam, He*, /**/ T**);

#undef T
