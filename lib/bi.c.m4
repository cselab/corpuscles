include(`co.m4')dnl
h_define(`List', `cortez, cortez_fm, cortez_zero, tri, self_circle')dnl
h_define(`foreach', `h_foreach(`n', `List', `$1')')dnl
h_define(`cap', `h_translit(`'capitalize(h_translit($1, `_', ` ')), ` ', `')')dnl
h_define(`type', `Bi`'cap($1)')dnl
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include "real.h"
#include "co/container.h"
#include "co/err.h"
#include "co/bi.h"
#include "co/macro.h"
#include "co/memory.h"
#include "co/util.h"
foreach(
``#include' "co/bi/n.h"
')dnl

#define T BI
#define SIZE (4048)
static char Names[SIZE];

foreach(
`static int n`_argv'(char***, He*, T**);
static int n`_fin'(T*);
static int n`_single'(T*, He*, real, const real*, const real*, const real*, const real*, const real*, const real*, real*, real*, real*);
static int n`_double'(T*, He*, real, const real*, const real*, const real*, const real*, const real*, const real*, real*, real*, real*);
static int n`_update'(T*, He*, const real*, const real*, const real*);
static void* n`_pointer'(T*);
')dnl

typedef struct Vtable Vtable;
struct T
{
        struct Vtable *vtable;
        const char *name;
};
struct Vtable {
        int (*argv)(char***, He*, T**);
        int (*fin)(T*);
        int (*single)(T*, He*, real, const real*, const real*, const real*, const real*, const real*, const real*, real*, real*, real*);
        int (*double0)(T*, He*, real, const real*, const real*, const real*, const real*, const real*, const real*, real*, real*, real*);
        int (*update)(T*, He*, const real*, const real*, const real*);
        void *(*pointer)(T*);
};

foreach(
`static Vtable n`_vtable' =
{
        n`_argv',
        n`_fin',
        n`_single',
        n`_double',
        n`_update',
        n`_pointer',
};
')dnl

static const char *Name[] =
{
foreach(
`"n",
')dnl
};

typedef int (*TypeArgv)(char***, He*, T**);
static const TypeArgv Argv[] =
{
foreach(
`n`_argv',
')dnl
};

foreach(
`h_pushdef(`N', `cap(n)')dnl
h_pushdef(`F', `type(n)')dnl
typedef struct N N;
struct N
{
        T global;
        F *local;
};
h_popdef(`N')dnl
h_popdef(`F')dnl
')dnl

foreach(
`h_pushdef(`N', `cap(n)')dnl
static int
n`_argv'(char ***p, He *he,  T **pq)
{
        N *q;
        MALLOC(1, &q);
        q->global.vtable = &n`_vtable';
        *pq = &q->global;
        return `bi_'n`_argv'(p, he, &q->local);
}

static int
n`_fin'(T *q)
{
        int status;
        N *b = CONTAINER_OF(q, N, global);
        status = `bi_'n`_fin'(b->local);
        FREE(q);
        return status;
}

static int
n`_single'(T *q, He *he, real alpha, const real *x, const real *y, const real *z, const real *fx, const real *fy, const real *fz, real *vx, real *vy, real *vz)
{
        N *b = CONTAINER_OF(q, N, global);
        return `bi_'n`_single'(b->local, he, alpha, x, y, z, fx, fy, fz, vx, vy, vz);
}

static int
n`_double'(T *q, He *he, real alpha, const real *x, const real *y, const real *z, const real *vx, const real *vy, const real *vz, real *ux, real *uy, real *uz)
{
        N *b = CONTAINER_OF(q, N, global);
        return `bi_'n`_double'(b->local, he, alpha, x, y, z, vx, vy, vz, ux, uy, uz);
}

static int
n`_update'(T *q, He *he, const real *x, const real *y, const real *z)
{
        N *b = CONTAINER_OF(q, N, global);
        return `bi_'n`_update'(b->local, he, x, y, z);
}

void *
n`_pointer'(T *q)
{
        N *b = CONTAINER_OF(q, N, global);
        return b->local;
}

h_popdef(`N')dnl
')dnl

int
bi_argv(const char *name, char ***parg, He *he, T **pq)
{
        int status;
        T *q;
        int i, n;

        n = sizeof(Name)/sizeof(Name[0]);
        for (i = 0; i < n; i++)
                if (util_eq(name, Name[i])) {
                        status = Argv[i](parg, he, &q);
                        q->name = Name[i];
                        *pq = q;
                        return status;
                }
        MSG("unknown BI: '%s'", name);
        MSG("possible values:");
        for (i = 0; i < n; i++)
                MSG("%s", Name[i]);
        ERR(CO_INDEX, "");
}


const char*
bi_list()
{
        const char *sep = "/";
        int i, n;
        n = sizeof(Name)/sizeof(Name[0]);
        Names[0] = '\0';
        for (i = 0; i < n; i++) {
                if (i > 0) strncat(Names, sep, SIZE - 1);
                strncat(Names, Name[i], SIZE - 1);
        }
        return Names;
}

int
bi_fin(T *q)
{
        return q->vtable->fin(q);
}

int
bi_single(T *q, He *he, real alpha, const real *x, const real *y, const real *z, const real *fx, const real *fy, const real *fz, real *vx, real *vy, real *vz)
{
        return q->vtable->single(q, he, alpha, x, y, z, fx, fy, fz, vx, vy, vz);
}

int
bi_double(T *q, He *he, real alpha, const real *x, const real *y, const real *z, const real *vx, const real *vy, const real *vz, real *ux, real *uy, real *uz)
{
        return q->vtable->double0(q, he, alpha, x, y, z, vx, vy, vz, ux, uy, uz);
}

int
bi_update(T *q, He *he, const real *x, const real *y, const real *z)
{
        return q->vtable->update(q, he, x, y, z);
}

const char*
bi_name(T *q)
{
        return q->name;
}

int
bi_good(const char *name)
{
        int i, n;
        n = sizeof(Name)/sizeof(Name[0]);
        for (i = 0; i < n; i++)
                if (util_eq(name, Name[i]))
                        return 1;
        return 0;
}

void*
bi_pointer(T *q)
{
        return q->vtable->pointer(q);
}
