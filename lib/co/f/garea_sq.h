#define T HeFGareaSq

typedef struct He He;
typedef struct T T;

/* area = sum_{tri} area_{tri}
   E = K * area^2 */
int he_f_garea_sq_ini(real K, He *, T **);
int he_f_garea_sq_argv(char ***, He *, T **);
int he_f_garea_sq_fin(T *);
int he_f_garea_sq_force(T *, He *, const real *, const real *,
                        const real *, /**/ real *, real *, real *);
real he_f_garea_sq_energy(T *, He *, const real *, const real *,
                          const real *);

#undef T
