#define T HeFVolume

typedef struct He He;
typedef struct T T;

/* E = K * (volume - volume0)^2 */
int he_f_volume_ini(real v0, real K, He*, T**);
int he_f_volume_fin(T*);

int he_f_volume_force(T*, He*,
                    const real *x, const real *y, const real *z, /**/
                    real *fx, real *fy, real *fz);
real he_f_volume_energy(T*, He*, const real *x, const real *y, const real *z);
int he_f_volume_v(T*, /**/ real  **v);

#undef T
