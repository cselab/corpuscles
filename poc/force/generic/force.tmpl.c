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
#define SIZE (4048)

#include "he/force.h"

#define T Force

//%begin
static int force_%name%_ini(void *param[], He*, /**/ T**);
//%end

struct T
{
    struct Vtable *vtable;
    const char *name;
};

#define SIZE (4048)
static char List[SIZE];
typedef int (*TypeIni)(void**, He*, T**);

static const char *Name[] = {
//%name
};

static int Narg[] = {
//%narg
};

static const TypeIni Ini[] = {
//%ini
};

int force_ini(const char *name, void **param, He *he, T **pq)
{
    int status;
    T *q;
    const int n = sizeof(Name)/sizeof(Name[0]);
    int i;
    for (i = 0; i < n; i++)
        if (util_eq(name, Name[i])) {
            status = Ini[i](param, he, &q);
            q->name = Name[i];
            *pq = q;
            return status;
        }
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

static int scl(const char **pargv[], real *p) {
    const char **argv;
    argv = *pargv;
    if (*argv == NULL)
        ERR(HE_IO, "not enough args");
    if (sscanf(*argv, XE_REAL_IN, p) != 1)
        ER("not a number '%s'", *argv);
    argv++;

    *pargv = argv;
    return HE_OK;
}

static int str(const char **pargv[], char *p) {
    const char **argv;
    argv = *pargv;

    if (*argv == NULL)
        ERR(HE_IO, "not enough args");

    strncpy(p, *argv, SIZE - 1);
    argv++;
    *pargv = argv;
    return HE_OK;
}

int force_argv(const char **pargv[], He *he, /**/ T **pq) {
    real   param[999];
    void *vparam[999];
    char name[SIZE];
    int narg, i;

    str(pargv, name);
    narg = force_narg(name);
    for (i = 0; i < narg; i++) {
        scl(pargv, &param[i]);
        vparam[i] = &param[i];
    }
    return force_ini(name, vparam, he,  pq);
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

const char *force_name(T *q)
{
    return q->name;
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
int force_%name%_ini(void *param[], He *he, /**/ T **pq)
{
    %Name% *q;
//%decl
    MALLOC(1, &q);
    q->force.vtable = &%name%_vtable;
    *pq = &q->force;
    return %ini%(%splice% he, &q->local);
}
//%end
