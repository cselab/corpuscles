typedef struct Skel Skel;
typedef struct Force2 Force2;

/* finite difference */
int fd2(Force2 *, Skel *, real delta, real *, real *, /**/ real *, real *);
