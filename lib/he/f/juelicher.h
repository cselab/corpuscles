#define T HeFJuelicher

typedef struct He He;
typedef struct T T;

int he_f_juelicher_ini(real K, He*, T**);
int he_f_juelicher_fin(T*);

int he_f_juelicher_force(T*, He*,
                      const real *x, const real *y, const real *z, /**/
                      real *fx, real *fy, real *fz);
real he_f_juelicher_energy(T*, He*, const real *x, const real *y, const real *z);

#undef T
