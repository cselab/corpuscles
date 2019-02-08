typedef struct He He;

int y_ini(const char *off, /**/ He **he, real **x, real **y, real **z);
int y_inif(FILE *f, /**/ He **he, real **x, real **y, real **z);

int y_fin(He *he, real *x, real *y, real *z);
