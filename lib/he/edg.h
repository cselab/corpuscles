#define T He

typedef struct T T;

int he_edg_ini(int n, T**);

/* edg[i, j] = v */
int he_edg_set(T*, int i, int j, int v);

/* ans = edg[i, j], ans = -1 if unset  */
int he_edg_get(T*, int i, int j);
int he_edg_fin(T*);

#undef T
