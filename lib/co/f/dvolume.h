#define T HeFDvolume

typedef struct He He;
typedef struct T T;

/* E = volume */
int he_f_dvolume_ini(He *, T **);
int he_f_dvolume_argv(char ***, He *, T **);
int he_f_dvolume_fin(T *);
int he_f_dvolume_force(T *, He *, const real *, const real *, const real *,
                       /**/ real *, real *, real *);
real he_f_dvolume_energy(T *, He *, const real *, const real *,
                         const real *);
int he_f_dvolume_v(T *, /**/ real ** v);

#undef T
