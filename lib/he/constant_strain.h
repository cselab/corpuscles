#define T ConstantStrain
#define P ConstantStrainParam

typedef struct T T;
typedef struct P P;
struct P { real Ka, Ks; };

int constant_strain_ini(const char *name, P, /**/ T**);
int constant_strain_fin(T*);

int constant_strain_force(T*,
                          const real a0[3], const real b0[3], const real c0[3],
                          const real a[3], const real b[3], const real c[3], /**/
                          real da[3], real db[3], real dc[3]);

real constant_strain_energy(T*,
                            const real a0[3], const real b0[3], const real c0[3],
                            const real a[3], const real b[3], const real c[3]);

#undef T
#undef P
