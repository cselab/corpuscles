#define T HeFGompperXin

typedef struct He He;
typedef struct T T;

int he_f_gompper_xin_ini(real Kb, real C0, real Kad, real DA0D, He*, T**);
int he_f_gompper_xin_fin(T*);
int he_f_gompper_xin_force(T*, He*,
                      const real *x, const real *y, const real *z, /**/
                      real *fx, real *fy, real *fz);
real he_f_gompper_xin_energy(T*, He*, const real *x, const real *y, const real *z);

#undef T
