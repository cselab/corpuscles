#define T

typedef struct T T;

int ring_ini(/**/ T**);

int ring_alpha(T*, const int*, const real*, const real*, const real*, /**/ real**);
int ring_beta(T*, const int*, const real*, const real*, const real*, /**/ real**);
int ring_theta(T*, const int*, const real*, const real*, const real*, /**/ real**);
int ring_xyz(T*, const int*, const real*, const real*, const real*, /**/ real**);
int ring_A(T*, const int*, const real*, const real*, const real*, /**/ real**);

int ring_fin(T);

#undef T
