typedef struct Alist Alist;

#define T Clist
typedef struct T T;

int clist_ini(int, T **);
int clist_alist(T *, Alist *);
int clist_fin(T *);
int clist_reset(T *);
int clist_push(T *, int cell, int part);
int clist_len(T *, int);
int clist_cells(T *, int cell, int **);
int clist_parts(T *, int cell, int **);
int clist_fwrite(FILE *, T *);

int clist_gen_n(int, T **);
int clist_gen_p(int, T **);

int clist_gen_nn(int, int, T **);
int clist_gen_np(int, int, T **);
int clist_gen_pn(int, int, T **);
int clist_gen_pp(int, int, T **);

int clist_gen_nnn(int, int, int, T **);
int clist_gen_nnp(int, int, int, T **);
int clist_gen_npn(int, int, int, T **);
int clist_gen_npp(int, int, int, T **);
int clist_gen_pnn(int, int, int, T **);
int clist_gen_pnp(int, int, int, T **);
int clist_gen_ppn(int, int, int, T **);
int clist_gen_ppp(int, int, int, T **);

#undef T
