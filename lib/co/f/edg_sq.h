#define T HeFEdgSq

typedef struct He He;
typedef struct T T;

int he_f_edg_sq_ini(real K, He *, T **);
int he_f_edg_sq_argv(char ***, He *, T **);
int he_f_edg_sq_fin(T *);
int he_f_edg_sq_force(T *, He *, const real *, const real *, const real *,
                      /**/ real *, real *, real *);
real he_f_edg_sq_energy(T *, He *, const real *, const real *,
                        const real *);
int he_f_edg_sq_e(T *, /**/ real ** v);

#undef T
