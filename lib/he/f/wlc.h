#define T HeFWlc

typedef struct He He;
typedef struct T T;

int he_f_wlc_ini(real x0, real K, const real *x, const real *y, const real *z, He*, T**);
int he_f_wlc_fin(T*);
int he_f_wlc_force(T*, He*, const real *x, const real *y, const real *z, /**/ real *fx, real *fy, real *fz);
real he_f_wlc_energy(T*, He*, const real *x, const real *y, const real *z);
int he_f_wlc_e(T*, /**/ real  **v);

#undef T
