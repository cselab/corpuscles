#include <stdio.h>
#include <tgmath.h>
#include <real.h>

#include <co/arc.h>
#include <co/err.h>

#define T Skel
typedef struct T T;
int skel_ini(int nv, T**);
int skel_open_ini(int nv, T**);
int skel_read(FILE*, /**/ real**, real**, T**);
int skel_fin(T*);
int skel_xy_fin(real*, real*, T*);

int skel_write(T*, const real*, const real*, FILE*);
int skel_nv(T*);
int skel_ne(T*);
int skel_edg_ij(T*, int, /**/  int*, int*);
#undef T

#define PI (3.141592653589793)
int
main(void)
{
	
}
