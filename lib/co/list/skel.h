#define T Skel2List

typedef struct T T;
typedef struct Skel Skel;
int skel_list_ini(const real lo[2], const real hi[2], real size, T**);
int skel_list_fin(T*);

int skel_list_push(T*, Skel*, const real*, const real*);
int skel_list_get(T*, real, real, real);
int skel_list_status(T*);
int skel_list_edg(T*);
int skel_list_point(T*, /**/ real[3]);

#undef T
