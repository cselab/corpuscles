#define T Restore
typedef struct He He;
typedef struct T T;
int restore_ini(real volume, He *, /**/ T **);
int restore_fin(T *);
int restore_volume(T *, He *, /*io */ real * x, real * y, real * z);

#undef T
