#define T Bending

typedef struct He He;
typedef struct T T;

struct BendingParam { real K, C0, Kad; };
typedef struct BendingParam BendingParam;

int bending_force(T*, He*, const real *x, const real *y, const real *z, /**/ real *fx, real *fy, real *fz);
real bending_energy(T*, He*, const real *x, const real *y, const real *z);
int bending_energy_ver(T*, /**/ real**);
int bending_fin(T*);


int bending_kantor_ini(const char*, BendingParam, He*, /**/ T**);

#undef T
