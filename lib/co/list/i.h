#define T Ilist
typedef struct T T;

enum {
    ILIST_END = -1
};
int ilist_ini(T **);
int ilist_fin(T *);
int ilist_push(T *, int);
int ilist_push_uniq(T *, int);
int ilist_len(T *);
int ilist_head(T *, int **);
int ilist_reset(T *);
int ilist_fwrite(FILE *, T *);

#undef T
