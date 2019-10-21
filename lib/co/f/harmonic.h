#define T HeFHarmonic

typedef struct He He;
typedef struct T T;

/* E = K/edg0 * sum_{edg} (edg - edg0)^2 */
int he_f_harmonic_ini(real e0, real K, He *, T **);
int he_f_harmonic_argv(char ***, He *, T **);
int he_f_harmonic_fin(T *);
int he_f_harmonic_force(T *, He *, const real *, const real *,
                        const real *, /**/ real *, real *, real *);
real he_f_harmonic_energy(T *, He *, const real *, const real *,
                          const real *);
int he_f_harmonic_e(T *, /**/ real ** v);

#undef T
