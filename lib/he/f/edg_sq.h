#define T HeFEdgSq

typedef struct He He;
typedef struct T T;

/* E = K * sum_{edg} edg^2 */
int he_f_edg_sq_ini(real K, He*, T**);
int he_f_edg_sq_fin(T*);

int he_f_edg_sq_force(T*, He*,
                    const real *x, const real *y, const real *z, /**/
                    real *fx, real *fy, real *fz);
real he_f_edg_sq_energy(T*, He*, const real *x, const real *y, const real *z);
int he_f_edg_sq_e(T*, /**/ real  **v);

#undef T
