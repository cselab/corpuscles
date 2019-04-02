#define T Bbox
typedef struct T T

struct T {
	real lo[3];
	real hi[3];
}

bbox_ini(int, const real *x, const real *y, const real *z, /**/ T*);
