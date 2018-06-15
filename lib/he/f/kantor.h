#define T HeFKantor

typedef struct He He;
typedef struct T T;

/* E = K/kantor0 * (kantor - kantor0)^2 */
int he_f_kantor_ini(real v0, real K, He*, T**);
int he_f_kantor_fin(T*);

int he_f_kantor_force(T*, He*,
                    const real *x, const real *y, const real *z, /**/
                    real *fx, real *fy, real *fz);
real he_f_kantor_energy(T*, He*, const real *x, const real *y, const real *z);

int he_f_kantor_cos(T*, /**/ real  **v);

#undef T
