#define T HePly

typedef struct He He;
typedef struct T T;

int ply_fread(FILE*, T**);
int ply_fin(T*);

int ply_nv(T*);
int ply_nt(T*);
int ply_nm(T*);

int ply_he(T*, He**);

int ply_tri(T*, int**);
int ply_x(T*, int m, real**);
int ply_y(T*, int m, real**);
int ply_z(T*, int m, real**);

#undef T
