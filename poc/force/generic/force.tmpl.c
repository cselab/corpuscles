#include <stdio.h>
#include <stddef.h>
#include <string.h>

#include "real.h"

#include "he/memory.h"
#include "he/err.h"
#include "he/he.h"
#include "he/container.h"
#include "he/macro.h"
#include "he/util.h"

//%header

#include "he/force.h"

#define T Force

struct T
{
    struct Vtable *vtable;
};

#define SIZE (4048)
static char List[SIZE];
typedef int (*TypeIni)(const real*, He*, T**);

static const char *Name[] = {
//%name
};

static int Narg[] = {
//%narg
};

static const TypeIni Ini[] = {
//%ini
};

int force_ini(const char *name, const real *param, He *he, T **pq)
{
    const int n = sizeof(Name)/sizeof(Name[0]);
    int i;
    for (i = 0; i < n; i++)
        if (util_eq(name, Name[i]))
            return Ini[i](param, he, pq);
    MSG("unknown force: '%s'", name);
    MSG("possible values:");
    for (i = 0; i < n; i++)
        MSG("%s", Name[i]);
    ERR(HE_INDEX, "");
}

int force_narg(const char *name)
{
    const int n = sizeof(Name)/sizeof(Name[0]);
    int i;
    for (i = 0; i < n; i++)
        if (util_eq(name, Name[i]))
            return Narg[i];
    MSG("unknown force: '%s'", name);
    MSG("possible values:");
    for (i = 0; i < n; i++)
        MSG("%s", Name[i]);
    ERR(HE_INDEX, "");
}


const char *force_list()
{
    const int n = sizeof(Name)/sizeof(Name[0]);
    const char *sep = "/";
    int i;
    List[0] = '\0';
    for (i = 0; i < n; i++) {
        if (i > 0) strncat(List, sep, SIZE - 1);
        strncat(List, Name[i], SIZE - 1);
    }
    return List;
}

typedef struct Vtable Vtable;
struct Vtable {
    int (*fin)(T*);
    int (*force)(T*, He*, const real *x, const real *y, const real *z, /**/ real *fx, real *fy, real *fz);
    real (*energy)(T*, He*, const real *x, const real *y, const real *z);
};

int force_fin(T *q)
{
    return q->vtable->fin(q);
}

int force_force(T *q, He *he, const real *x, const real *y, const real *z, /**/ real *fx, real *fy, real *fz)
{
    return q->vtable->force(q, he, x, y, z, fx, fy, fz);
}

real force_energy(T *q, He *he, const real *x, const real *y, const real *z)
{
    return q->vtable->energy(q, he, x, y, z);
}

//%begin
typedef struct %Name% %Name%;
struct %Name% {
    T force;
    %type% *local;
};
static int %name%_fin(T *q)
{
    int status;
    %Name% *b = CONTAINER_OF(q, %Name%, force);
    status = %fin%(b->local);
    FREE(q);
    return status;
}
static int %name%_force(T *q, He *he, const real *x, const real *y, const real *z,
                               /**/ real *fx, real *fy, real *fz)
{
    %Name% *b = CONTAINER_OF(q, %Name%, force);
    return %force%(b->local, he, x, y, z, /**/ fx, fy, fz);
}
static real %name%_energy(T *q, He *he, const real *x, const real *y, const real *z)
{
    %Name% *b = CONTAINER_OF(q, %Name%, force);
    return %energy%(b->local, he, x, y, z);
}
static Vtable %name%_vtable = {
    %name%_fin,
    %name%_force,
    %name%_energy,
};
int force_%name%_ini(const real *param, He *he, /**/ T **pq)
{
    %Name% *q;
//%decl
    MALLOC(1, &q);
    q->force.vtable = &%name%_vtable;
    *pq = &q->force;
    return %ini%(%splice% he, &q->local);
}
//%end
