#define T HeFGareaZero

typedef struct He He;
typedef struct T T;

/* area = sum_{tri} area_{tri}
   E = K * area */
int he_f_garea_zero_ini(real K, He *, T **);
int he_f_garea_zero_argv(char ***, He *, T **);
int he_f_garea_zero_fin(T *);
int he_f_garea_zero_force(T *, He *, const real *, const real *,
                        const real *, /**/ real *, real *, real *);
real he_f_garea_zero_energy(T *, He *, const real *, const real *,
                          const real *);

#undef T
