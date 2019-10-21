#define T Sph2Plane

typedef struct T T;
int sph_2plane_ini(real r, T **);
int sph_2plane_fin(T *);
int sph_2plane_apply(T *, real d0, real d1,
                     real(*)(real x, real y, real z, void *), void *,
                     /**/ real *);
int sph_2plane_volume(T *, real d0, real d1, /**/ real *);

#undef T
