#define T HeFVolumeNormal

typedef struct He He;
typedef struct T T;

int he_f_volume_normal_ini(real a0, real K, He *, T **);
int he_f_volume_normal_argv(char ***, He *, T **);
int he_f_volume_normal_fin(T *);
int he_f_volume_normal_force(T *, He *, const real *, const real *,
                             const real *, /**/ real *, real *, real *);
real he_f_volume_normal_energy(T *, He *, const real *, const real *,
                               const real *);

#undef T
