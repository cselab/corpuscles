include(`co.m4')dnl
h_define(`List', `area, glen')dnl
h_define(`foreach', `h_foreach(`n', `List', `$1')')dnl
h_define(`f2cap', `h_translit(`'capitalize(h_translit($1, `_', ` ')), ` ', `')')dnl
h_define(`f2type', `F2`'f2cap($1)')dnl
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include "real.h"
#include "co/container.h"
#include "co/err.h"
#include "co/force2.h"
#include "co/macro.h"
#include "co/memory.h"
#include "co/util.h"
foreach(
``#include' "co/f2/n.h"
')dnl

#define T Force2
#define SIZE (4048)
static char Names[SIZE];

foreach(
`static int n`_argv'(char***, Skel*, T**);
static int n`_fin'(T*);
static int n`_force'(T*, Skel*, const real*, const real*, real*, real*);
static real  n`_energy'(T*, Skel*, const real*, const real*);
static void* n`_pointer'(T*);
')dnl

typedef struct Vtable Vtable;
struct T
{
	struct Vtable *vtable;
	const char *name;
};
struct Vtable {
	int (*argv)(char***, Skel*, T**);
	int (*fin)(T*);
	int (*force)(T*, Skel*, const real*, const real*, real*, real*);
	real  (*energy)(T*, Skel*, const real*, const real*);
	void* (*pointer)(T*);
};

foreach(
`static Vtable n`_vtable' =
{
	n`_argv',
	n`_fin',
	n`_force',
	n`_energy',
	n`_pointer',
};
')dnl

static const char *Name[] =
{
foreach(
`"n",
')dnl
};

typedef int (*TypeArgv)(char***, Skel*, T**);
static const TypeArgv Argv[] =
{
foreach(
`n`_argv',
')dnl
};

foreach(
`h_pushdef(`N', `f2cap(n)')dnl
h_pushdef(`F', `f2type(n)')dnl
typedef struct N N;
struct N
{
	T force;	
    	F *local;
};
h_popdef(`N')dnl
h_popdef(`F')dnl
')dnl


foreach(
`h_pushdef(`N', `f2cap(n)')dnl
static int
n`_argv'(char ***p, Skel *skel,  T **pq)
{
	N *q;
	MALLOC(1, &q);
	q->force.vtable = &n`_vtable';
	*pq = &q->force;
	return `f2_'n`_argv'(p, skel, &q->local);
}

static int
n`_fin'(T *q)
{
	int status;
	N *b = CONTAINER_OF(q, N, force);
	status = `f2_'n`_fin'(b->local);
	FREE(q);
	return status;
}

static int
n`_force'(T *q, Skel *skel, const real *x, const real *y, real *fx, real *fy)
{
	N *b = CONTAINER_OF(q, N, force);
	return `f2_'n`_force'(b->local, skel, x, y, fx, fy);
}

static real
n`_energy'(T *q, Skel *skel, const real *x, const real *y)
{
	N *b = CONTAINER_OF(q, N, force);
	return `f2_'n`_energy'(b->local, skel, x, y);
}

static void*
n`_pointer'(T *q)
{
	N *b = CONTAINER_OF(q, N, force);
	return b->local;
}
h_popdef(`N')dnl
')dnl

int
force2_argv(const char *name, char ***parg, Skel *skel, T **pq)
{
	int status;
	T *q;
	int i, n;

	n = sizeof(Name)/sizeof(Name[0]);
	for (i = 0; i < n; i++)
		if (util_eq(name, Name[i])) {
			status = Argv[i](parg, skel, &q);
			q->name = Name[i];
			*pq = q;
			return status;
		}
	MSG("unknown force2: '%s'", name);
	MSG("possible values:");
	for (i = 0; i < n; i++)
		MSG("%s", Name[i]);
	ERR(CO_INDEX, "");
}


const char*
force2_list()
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
force2_fin(T *q)
{
	return q->vtable->fin(q);
}

int
force2_force(T *q, Skel *skel, const real *x, const real *y, real *fx, real *fy)
{
	return q->vtable->force(q, skel, x, y, fx, fy);
}

real
force2_energy(T *q, Skel *skel, const real *x, const real *y)
{
	return q->vtable->energy(q, skel, x, y);
}

const char*
force2_name(T *q)
{
	return q->name;
}

void*
force2_pointer(T *q)
{
	return q->vtable->pointer(q);
}

