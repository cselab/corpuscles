#define T HeFAreaEq

typedef struct He He;
typedef struct T T;

/* 
A = sum_{tri} area_tri
E = K*n_tri/A*sum_{tri}(area_{tri} - A/n_tri)^2 */
int he_f_area_eq_ini(real a0, real K, He *, T **);
int he_f_area_eq_argv(char ***, He *, T **);
int he_f_area_eq_fin(T *);
int he_f_area_eq_force(T *, He *, const real *, const real *, const real *,
                       /**/ real *, real *, real *);
real he_f_area_eq_energy(T *, He *, const real *, const real *,
                         const real *);

#undef T
