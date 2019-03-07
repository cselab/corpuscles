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

int ring_xu(int, const real *xyz, const real *C, /**/ real[3]);
int ring_xv(int, const real *xyz, const real *C, /**/ real[3]);

int ring_xuu(int, const real *xyz, const real *C, /**/ real[3]);
int ring_xuv(int, const real *xyz, const real *C, /**/ real[3]);
int ring_xvv(int, const real *xyz, const real *C, /**/ real[3]);

int ring_gcov(int, const real *xyz, const real *C, /**/ real[3]);
real ring_g(int, const real *xyz, const real *C);
int ring_gcnt(int, const real *xyz, const real *C, /**/ real[3]);

#undef T
