#define T SphPlane

typedef struct T T;
int sph_plane_ini(real r, T**);
int sph_plane_apply(T*, real d, real (*)(real x, real y, real z, real d, void*), void*, /**/ real*);
int sph_plane_fin(T*);

#undef T