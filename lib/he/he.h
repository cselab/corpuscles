#define T He

typedef struct T T;
typedef struct HeRead HeRead;

int he_ini(HeRead*, T**);
int he_fin(T*);

int he_nxt(T*);
int he_flp(T*);
int he_ver(T*);
int he_tri(T*);
int he_edg(T*);

int he_hdg_ver(T*);
int he_hdg_edg(T*);
int he_hdg_tri(T*);

#undef T
