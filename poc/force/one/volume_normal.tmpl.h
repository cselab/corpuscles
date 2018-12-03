#define T HeF%Name%

typedef struct He He;
typedef struct T T;

int he_f_%name%_ini(real a0, real K, He*, T**);
int he_f_%name%_fin(T*);
int he_f_%name%_force(T*, He*,
                      const real *x, const real *y, const real *z, /**/
                      real *fx, real *fy, real *fz);
real he_f_%name%_energy(T*, He*, const real *x, const real *y, const real *z);

#undef T
