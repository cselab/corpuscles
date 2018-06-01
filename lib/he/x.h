extern const real pi;
extern int NV, NE, NT, NH;
extern int *T0, *T1, *T2;
extern int *D0, *D1, *D2, *D3;
extern real *XX, *YY, *ZZ;
extern real *RR; /* sqrt(x^2 + y^2 + z^2) */
extern real *TH; /* acos(z/r) */

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

/* is bondary? */
int  bnd(int h);

/* alloca and zero */
int RZERO(int, real**);
