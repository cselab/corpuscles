#define T IntegralTri

typedef struct T T;
int integral_tri_ini(T**);
int integral_tri_fin(T*);
int integral_tri_apply(T*, real[3], real[3], real[3], real (*)(real x, real y, real, void*), void*, /**/ real*);

#undef T
