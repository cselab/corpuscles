#define T He
typedef struct T T;
typedef struct HeRead HeRead;

int he_ini(HeRead *, T **);
int he_file_ini(const char *path, T **);
int he_tri_ini(int nv, int nt, int *tri, T **);

int he_fin(T *);

int he_nv(T *);                 /* number of vert, edg, edg, hdg */
int he_nt(T *);
int he_ne(T *);
int he_nh(T *);

int he_nxt(T *, int h);         /* return half-edg */
int he_flp(T *, int h);

int he_ver(T *, int h);         /* return ver, tri, or edg */
int he_tri(T *, int h);
int he_edg(T *, int h);
int he_hdg_ver(T *, int v);     /* return half-edg */
int he_hdg_edg(T *, int e);
int he_hdg_tri(T *, int t);

int he_bnd(T *, int h);         /* 1 for boundary */
int he_ijk(T *, int h, /**/ int *, int *, int *);
int he_tri_ijk(T *, int t, /**/ int *, int *, int *);
int he_edg_ij(T *, int e, /**/ int *, int *);
int he_dih_ijkl(T *, int e, /**/ int *, int *, int *, int *);

int he_edg_rotate(T *, int e);  /* AKA flip edge */
int he_edg_split(T *, int e);
int he_tri_split(T *, int t);

int he_E(T *, int **, int **);
int he_T(T *, int **, int **, int **);
int he_D(T *, int **, int **, int **, int **);

int he_bnd_ver(T *, int v);
int he_ring(T *, int v, /**/ int *n, int **ring);

#undef T
