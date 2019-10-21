#define T HeFRvolume

typedef struct He He;
typedef struct T T;

/* E = reduced volume */
int he_f_rvolume_ini(He *, T **);
int he_f_rvolume_argv(char ***, He *, T **);
int he_f_rvolume_fin(T *);
int he_f_rvolume_force(T *, He *, const real *, const real *, const real *,
                       /**/ real *, real *, real *);
real he_f_rvolume_energy(T *, He *, const real *, const real *,
                         const real *);

#undef T
