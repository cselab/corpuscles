#define T Orient
typedef struct He He;
typedef struct Ten Ten;
typedef struct T T;
int orient_ini(He *, /**/ T **);
int orient_fin(T *);
int orient_apply(T *, /*io */ real * x, real * y, real * z);
int orient_transform(T *, /**/ Ten * t);

#undef T
