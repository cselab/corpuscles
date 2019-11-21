typedef struct He He;
int y_ini(const char *, /**/ He **, real **, real **, real **);
int y_inif(FILE *, /**/ He **, real **, real **, real **);
int y_inif2(FILE *, FILE *, /**/ He **, real **, real **, real **);
int y_inif2b(FILE *, FILE *, /**/ He **, He **h0, He **h1, real **h, real **, real **);
int y_fin2b(He *h0, He *h1, He *h, real *, real *, real *);
int y_fin(He *, real *, real *, real *);
