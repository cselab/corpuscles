#define T HeFHarmonicRef

typedef struct He He;
typedef struct T T;

/* E = K * sum_{edg} [ (edg - edg0)^2 / edg0  ]  */
int he_f_harmonic_ref_ini(real K, const real *x, const real *y, const real *z, He*, T**);
int he_f_harmonic_ref_fin(T*);
int he_f_harmonic_ref_force(T*, He*, const real *x, const real *y, const real *z, /**/ real *fx, real *fy, real *fz);
real he_f_harmonic_ref_energy(T*, He*, const real *x, const real *y, const real *z);
int he_f_harmonic_ref_e(T*, /**/ real  **v);

#undef T
