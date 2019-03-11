#define T Ring

typedef struct T T;

int ring_ini(/**/ T**);
int ring_fin(T*);

/* vertice, ring[], x, y, z */
int ring_alpha(T*, int, const int*, const real*, const real*, const real*, /**/ real**);
int ring_beta(T*, int, const int*, const real*, const real*, const real*, /**/ real**);
int ring_theta(T*, int, const int*, const real*, const real*, const real*, /**/ real**);
int ring_xyz(T*, int, const int*, const real*, const real*, const real*, /**/ real**);
int ring_A(T*, int, const int*, const real*, const real*, const real*, /**/ real**);
int ring_B(T*, int, const int*, const real*, const real*, const real*, /**/ real**);
int ring_C(T*, int, const int*, const real*, const real*, const real*, /**/ real**);

/* n, xyz, C */
int ring_xu(int, const real*, const real*, /**/ real[3]);
int ring_xv(int, const real*, const real*, /**/ real[3]);
int ring_xuu(int, const real*, const real*, /**/ real[3]);
int ring_xuv(int, const real*, const real*, /**/ real[3]);
int ring_xvv(int, const real*, const real*, /**/ real[3]);
int ring_gcov(int, const real*, const real*, /**/ real[3]);
real ring_g(int, const real*, const real*);
int ring_gcnt(int, const real*, const real*, /**/ real[3]);
int ring_normal(int, const real*, const real*, /**/ real[3]);

int ring_wgrad(T*, int, const real*, const real*, /**/ real**);

#undef T
