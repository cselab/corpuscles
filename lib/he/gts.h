#define T HeOff

typedef struct He He;

typedef struct T T;

int off_ini(const char *path, T**);
int off_fin(T*);

int off_nt(T*);
int off_nv(T*);
int off_ver(T*, real**);
int off_tri(T*, int**);
int off_xyz(T*, /**/ real*, real*, real*);

int off_fwrite(T*, const real*, const real*, const real*, FILE*);
int off_write0(T*, const real*, const real*, const real*, /**/ const char*);

int off_tri_fwrite(T*, const int *tri, FILE*);
int off_tri_write(T*, const int *tri, /**/ const char*);

int off_he_fwrite(T*, He*, FILE*);
int off_he_write(T*, He*, /**/ const char*);

int off_he_xyz_fwrite(He*, const real *x, const real *y, const real *z, /**/ FILE *f);
int off_he_xyz_write(He*, const real *x, const real *y, const real *z, /**/ const char*);

#undef T
