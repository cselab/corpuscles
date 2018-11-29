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

#include "he/f/area.h"
#include "he/force.h"

#define T Force

struct T {struct Vtable *vtable; };

#define SIZE (4048)
static char List[SIZE];
typedef int (*TypeIni)(const real*, He*, T**);

static const char *Name[] =
{
    "area",
};

static const TypeIni Ini[] =
{
    force_area_ini,
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
struct Vtable
{
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

typedef struct Area Area;
struct Area {
    T force;
    HeFArea *local;
};

static int area_fin(T *q) {
    int status;
    Area *b = CONTAINER_OF(q, Area, force);
    status = he_f_area_fin(b->local);
    FREE(q);
    return status;
}

static int area_force(T *q, He *he, const real *x, const real *y, const real *z,
                               /**/ real *fx, real *fy, real *fz) {
    Area *b = CONTAINER_OF(q, Area, force);
    return he_f_area_force(b->local, he, x, y, z, /**/ fx, fy, fz);
}

static real area_energy(T *q, He *he, const real *x, const real *y, const real *z) {
    Area *b = CONTAINER_OF(q, Area, force);
    return he_f_area_energy(b->local, he, x, y, z);
}

static Vtable area_vtable = {
    area_fin,
    area_force,
    area_energy
};
int force_area_ini(const real *param, He *he, /**/ T **pq) {
    real a0, K;
    Area *q;
    a0 = *param++;
    K = *param++;
    MALLOC(1, &q);
    q->force.vtable = &area_vtable;
    *pq = &q->force;
    return he_f_area_ini(a0, K, he, &q->local);
}
