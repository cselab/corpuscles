#define T Ply
typedef struct He He;
typedef struct T T;
int ply_fread(FILE *, T **);
int ply_fin(T *);
int ply_nv(T *);
int ply_nt(T *);
int ply_nm(T *);
int ply_he(T *, He **);
int ply_tri(T *, int **);
int ply_x(T *, int m, real **);
int ply_y(T *, int m, real **);
int ply_z(T *, int m, real **);
int ply_fwrite(T *, FILE *, int *ban);
int ply_vtk_txt(T *, FILE *, int *ban, real * scalar);
int ply_vtk_bin(T *, FILE *, int *ban, real * scalar);

#undef T
