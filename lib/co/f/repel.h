#define T HeFRepel

typedef struct He He;
typedef struct T T;

int he_f_repel_ini(real cutoff, real K, He *, T **);
int he_f_repel_argv(char ***, He *, T **);
int he_f_repel_fin(T *);
int he_f_repel_force(T *, He *, const real *, const real *, const real *,
                      /**/ real *, real *, real *);
real he_f_repel_energy(T *, He *, const real *, const real *,
                        const real *);

#undef T
