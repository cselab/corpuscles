#define T HeFMeyer

typedef struct He He;
typedef struct T T;

int he_f_meyer_ini(real K, real C0, real Kad, He*, T**);
int he_f_meyer_fin(T*);

int he_f_meyer_force(T*, He*,
                      const real *x, const real *y, const real *z, /**/
                      real *fx, real *fy, real *fz);
real he_f_meyer_energy(T*, He*, const real *x, const real *y, const real *z);

int he_f_meyer_cos(T*, /**/ real**);

#undef T
