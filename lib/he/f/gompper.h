#define T HeFGompper

typedef struct He He;
typedef struct T T;

/* E = 2 * K * sum_{dih} (1 - cos(ang)) */
int he_f_gompper_ini(real K, He*, T**);
int he_f_gompper_fin(T*);

int he_f_gompper_force(T*, He*,
                      const real *x, const real *y, const real *z, /**/
                      real *fx, real *fy, real *fz);
real he_f_gompper_energy(T*, He*, const real *x, const real *y, const real *z);

int he_f_gompper_cos(T*, /**/ real**);

#undef T
