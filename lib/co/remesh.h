typedef struct He He;
int remesh_loop(He *, real ** x, real ** y, real ** z, He **);
int remesh_sqrt3(He *, real ** x, real ** y, real ** z, He **);
int remesh_tri_split(int nmax, int *tri, int, He *, real **, real **,
                     real **);
