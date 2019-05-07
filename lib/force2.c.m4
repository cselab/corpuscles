include(`co.m4')dnl
#include <stdio.h>
#include <string.h>
#include "real.h"
#include "co/container.h"
#include "co/err.h"
#include "co/force2.h"
#include "co/macro.h"
#include "co/memory.h"
#include "co/util.h"

#include "co/f2/glen.h"
#include "co/f2/area.h"

#define T Force2

typedef struct Vtable Vtable;
struct T
{
	struct Vtable *vtable;
	const char *name;
};

struct Vtable {
    int (*fin)(T*);
    int (*force)(T*, Skel*, const real*, const real*, /**/ real*, real*);
    real  (*energy)(T*, Skel*, const real*, const real*);
    void* (*pointer)(T*);
};
