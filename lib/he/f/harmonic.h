#define T HeFHarmonic

typedef struct He He;
typedef struct T T;

/* E = K * sum_{edg} (edg - edg0)^2 */
int he_f_harmonic_ini(real v0, real K, He*, T**);
int he_f_harmonic_fin(T*);

int he_f_harmonic_force(T*, He*,
                    const real *x, const real *y, const real *z, /**/
                    real *fx, real *fy, real *fz);
real he_f_harmonic_energy(T*, He*, const real *x, const real *y, const real *z);
int he_f_harmonic_e(T*, /**/ real  **v);

#undef T
