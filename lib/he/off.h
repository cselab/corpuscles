#define T HeOff

typedef struct He He;

typedef struct T T;

int he_off_ini(const char *path, T**);
int he_off_fin(T*);

int he_off_nt(T*);
int he_off_nv(T*);
int he_off_ver(T*, real**);
int he_off_tri(T*, int**);
int he_off_xyz(T*, /**/ real*, real*, real*);

int he_off_fwrite(T*, const real*, const real*, const real*, FILE*);
int he_off_write(T*, const real*, const real*, const real*, /**/ const char*);

int he_off_tri_fwrite(T*, const int *tri, FILE*);
int he_off_tri_write(T*, const int *tri, /**/ const char*);

int he_off_he_fwrite(T*, He*, FILE*);
int he_off_he_write(T*, He*, /**/ const char*);

int he_off_he_xyz_fwrite(T*, He*, const real *x, const real *y, const real *z, /**/ FILE *f);
int he_off_he_xyz_write(T*, He*, const real *x, const real *y, const real *z, /**/ const char*);

#undef T
