#define T HeF%Name%

typedef struct He He;
typedef struct T T;

int he_f_%name%_ini(real Kb, real C0, real Kad, real DA0D, He*, T**);
int he_f_%name%_fin(T*);
int he_f_%name%_force(T*, He*,
                      const real *x, const real *y, const real *z, /**/
                      real *fx, real *fy, real *fz);
real he_f_%name%_energy(T*, He*, const real *x, const real *y, const real *z);

#undef T
