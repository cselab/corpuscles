#define T Force2

typedef struct Skel Skel;
typedef struct T T;

int force2_argv(const char *name, char***, Skel*, /**/ T**);
int force2_fin(T*);
int force2_force(T*, Skel*, const real*, const real*, /**/ real*, real*);
real force2_energy(T*, Skel*, const real*, const real*);

const char* force2_name(T*);
void* force2_pointer(T*);
const char* force2_list();
int force2_narg(const char*);

#undef T
