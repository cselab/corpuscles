#define T Bbox
typedef struct T T;

int bbox_ini(T**);
int bbox_fin(T*);

int bbox_update(T*, int, const real*, const real*, const real*);
int bbox_lo(T*, real**);
int bbox_hi(T*, real**);

