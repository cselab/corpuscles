#define T Bbox
typedef struct T T;

struct T {
	real lo[3];
	real hi[3];
};

int bbox_ini(int, const real*, const real*, const real*, /**/ T*);
