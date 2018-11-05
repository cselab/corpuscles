#define T HeFDvolume

typedef struct He He;
typedef struct T T;

/* E = volume */
int he_f_dvolume_ini(He*, T**);
int he_f_dvolume_fin(T*);

int he_f_dvolume_force(T*, He*,
                    const real *x, const real *y, const real *z, /**/
                    real *fx, real *fy, real *fz);
real he_f_dvolume_energy(T*, He*, const real *x, const real *y, const real *z);
int he_f_dvolume_v(T*, /**/ real  **v);

#undef T
