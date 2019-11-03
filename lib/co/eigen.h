#define T Eigen

typedef struct He He;
typedef struct T T;

int eigen_ini(He *, /**/ T **);
int eigen_fin(T *);

int eigen_vector(T *, real * x, real *, real *, /**/ real[9]);
int eigen_vector_surface(T *, He *, real *, real *, real*, /**/ real[9]);
int eigen_vector_tri(T *, He *, real *, real *, real *, /**/ real[9]);

int eigen_values(T *, /**/ real val[3]);

#undef T
