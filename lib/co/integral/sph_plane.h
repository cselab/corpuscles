#define T SphPlane

typedef struct T T;
int sph_plane_ini(real d, T **);
int sph_plane_fin(T *);
int sph_plane_apply(T *, real r, real(*)(real x, real y, real z, void *),
                    void *, /**/ real *);
int sph_plane_volume(T *, real d, /**/ real *);

#undef T
