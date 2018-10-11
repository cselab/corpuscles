#define T Bending

typedef struct He He;
typedef struct BendingParam BendingParam;
typedef struct T T;

struct BendingParam { real K, C0, Kad; };
int bending_ini(BendingParam p, He*, /**/ T**);
int bending_force(T*, He*, const real *x, const real *y, const real *z, /**/ real *fx, real *fy, real *fz);
real bending_energy(T*, He*, const real *x, const real *y, const real *z);
int bending_energy_ver(T*, /**/ real**);

int bending_fin(T*);

#undef T
