#define T Strain
#define P StrainParam
typedef struct T T;
typedef struct P P;
struct P {
    real Ka, Ks, mu, a3, a4, b1, b2;
};
int strain_ini(const char *name, P, /**/ T **);
int strain_fin(T *);
int strain_force(T *,
                 const real a0[3], const real b0[3], const real c0[3],
                 const real a[3], const real b[3], const real c[3], /**/
                 real da[3], real db[3], real dc[3]);
real strain_energy(T *,
                   const real a0[3], const real b0[3], const real c0[3],
                   const real a[3], const real b[3], const real c[3]);
int strain_energy_ab(T *,
                     const real a0[3], const real b0[3], const real c0[3],
                     const real a[3], const real b[3], const real c[3],
                     real * ea, real * eb);
#undef T
#undef P
