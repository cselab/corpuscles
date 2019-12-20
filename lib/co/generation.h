#define T Generation
typedef struct T T;
generation_ini(He *, T **)
generation_fin(T *)
generation_refine(T *, int t, He * he, real ** x, real ** y, real ** z)
generation_invariant(T * q, He * he)
generation_write(T * q, He * he, const real * x, const real * y, const real * z, FILE * file);		 
generation_read(FILE * file, He ** phe, real ** x, real ** y, real ** z, T ** pq);
#undef T
