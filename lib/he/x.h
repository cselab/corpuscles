extern int NV, NE, NT, NH;
extern int *T0, *T1, *T2;
extern int *D0, *D1, *D2, *D3;
extern real *XX, *YY, *ZZ;

int ini(const char *path);
int fin();
int  nxt(int h);
int  flp(int h);

int  ver(int h);
int  edg(int h);
int  tri(int h);

int  hdg_ver(int v);
int  hdg_edg(int e);
int  hdg_tri(int t);
int  bnd(int h);
