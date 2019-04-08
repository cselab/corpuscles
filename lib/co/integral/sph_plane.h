#define T SphPlane

typedef struct T T;
int sph_plane_ini(real r, real (*fun)(real x, real y, real z, real d, void*), void*, T**);
real sph_plane_apply(T*, real d);
int sph_plane_fin(T*);

#undef T