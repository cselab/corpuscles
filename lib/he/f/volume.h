#define T HeFVolume

typedef struct He He;
typedef struct T T;

/* E = K/volume0 * (volume - volume0)^2 */
int he_f_volume_ini(real v0, real K, He*, T**);
int he_f_volume_argv(const char***, He*, T**);
int he_f_volume_fin(T*);
int he_f_volume_set_v(T*, real); /* set v0 */
int he_f_volume_force(T*, He*, const real*, const real*, const real*, /**/ real*, real*, real*);
real he_f_volume_energy(T*, He*, const real*, const real*, const real*);
int he_f_volume_v(T*, /**/ real  **v);

#undef T
