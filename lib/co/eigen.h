#define T Eigen

typedef struct He He;
typedef struct Ten Ten;
typedef struct T T;

int eigen_ini(He*, /**/ T**);
int eigen_fin(T*);

int eigen_vector(T*, real *x, real *y, real *z, /**/ real v[9]);
int eigen_vector_surface(T*, He*, real *x, real *y, real *z, /**/ real v[9]);
int eigen_vector_tri(T*, He*, real *x, real *y, real *z, /**/ real v[9]);

#undef T
