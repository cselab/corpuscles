#define T Alist
typedef struct T T;
enum {
    ALIST_END = -1
};
int alist_ini(int, T **);
int alist_fin(T *);
int alist_reset(T *);
int alist_push(T *, int, int);
int alist_push_uniq(T *, int, int);
int alist_head(T *, int, int **);
int alist_len(T *, int);
int alist_n(T *);
int alist_fwrite(FILE *, T *);

#undef T
