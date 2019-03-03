![Intro image](img/rbc/sde/Da-0.358.png)
# Intro
A C library deal with triangulated surfaces shaped by forces
and constraints.

# Install

Minimal requirements `pkg-config`, `make`, `gcc-c`. Optional
requrements `gsl`, `geomview`, `atest`, `maxima`.

Download code from
```sh
$ git clone git@gitlab.ethz.ch:mavt-cse/he
```

Adjust `conf.mk` if you want to change defaults

```makefile
CC = c99
CFLAGS = -O2 -g
PREFIX = $(HOME)
MAXIMA_HOME = $(HOME)/.maxima
```

Library is installed under the path `PREFIX`. Add `PREFIX/bin` and
`PREFIX/man` to envariament variables `PATH` and `MANPATH`. Run
`make`.

Run
```sh
$ make
```

If you have `atest` installed you can run
```sh
$ make test
```

# Examples

### hello world

A simple example is in [example/hello](example/hello/)

```c
$ cat main.c
#include <stdio.h>

#include <real.h>
#include <he/tri.h>

#define FMT HE_REAL_OUT

int main(void) {
    enum {X, Y, Z};
    real a[3], b[3], c[3], A;

    a[X] = 0; a[Y] = 0; a[Z] = 0;
    b[X] = 1; b[Y] = 0; b[Z] = 0;
    c[X] = 0; c[Y] = 1; c[Z] = 0;

    A = tri_area(a, b, c);
    printf("Area is " FMT "\n", A);
}

```

```make
$ cat Makefile
include ../../conf.mk
PREC = d
HE_CFLAGS = `co.conf --cflags $(PREC)`
HE_LDFLAGS = `co.conf --libs $(PREC)`

main: main.c
	$(CC) main.c $(CFLAGS) $(HE_CFLAGS) $(LDFLAGS) $(HE_LDFLAGS) -o main
test:
install:

.PHONY: clean test install doc
clean:; rm -f main

```

### read off file

An example is in [example/off/read](example/off/read/)

```c
$ cat main.c
#include <stdio.h>
#include <stdlib.h>

#include <real.h>
#include <he/array.h>
#include <he/he.h>
#include <he/y.h>

#define FMT HE_REAL_OUT

int main(void) {
    real *x, *y, *z;
    real hi, lo;
    He *he;
    int nv;

    y_inif(stdin, &he, &x, &y, &z);
    nv = he_nv(he);
    hi = array_max(nv, x);
    lo = array_min(nv, x);

    printf("number of vertices is %d\n", nv);
    printf(FMT " " FMT "\n", hi, lo);
    y_fin(he, x, y, z);
}

```

# visualization

We use a wrapper to [geomview](http://geomview.org).

```sh
$ he.geomview -h
he.geomview [-t x y z] [-r x y z] [-a APPEARANCE] [-o FILE] [OFF]..
he geomview wrapper
-t x y z      translation
-r x y z      rotation in degree
-f zoom       field of view (default is 40)
-a APPEARANCE load appearance from a file
-o FILE       write FILE and exit
-O            write all PPM files and exit
-OO           write all oogl files and exit
-format	ppmscreen|ppm|ps|ppmosmesa|ppmosglx
-p command    process every off file by running 'command' < IN.off > OUT.off
-n none|each|all|keep normalization status (see geomview manual)
-c command    run command on every file and write output to stderr, %f is replaced by a file name
-i command    run command on every image, %i replaced by input; %o -- by output; %b --- by basename

Keys:
    q: quit
    s: save snap.ppm
    S: save every snapshot
    p: panel
    j/k: switch between off files
    J: dump file name
    [SPC]: dump orientation and field of view

Environment variables:
WX, WY: resolution of the snapshot (default: 800x600)
BACKGROUND: default ('1 1 1')

Examples:
he.geomview -t 0.25 0.25 0     data/rbc.off
he.geomview -a data/appearance data/rbc.off
he.geomview -o snap.ppm        data/rbc.off
he.geomview                    data/rbc.off data/sph.off
he.geomview -p he.orient       data/rbc.off data/sph.off
he.geomview -c off.volume      data/rbc.off data/sph.off

```

# Lib

## Floating point precision

[prec/s/real.h](lib/he/prec/s/real.h), [prec/d/real.h](lib/he/prec/d/real.h), [prec/l/real.h](lib/he/prec/l/real.h)
:   single, double, long double

## Math

[vec.h](lib/he/vec.h), [edg.h](lib/he/edg.h), [tri.h](lib/he/tri.h), [dih.h](lib/he/dih.h), [ten.h](lib/he/ten.h)
: vector, edges, triangels, dihidrals, tensors

[dvec.h](lib/he/dvec.h), [dedg.h](lib/he/dedg.h), [dtri.h](lib/he/dtri.h), [ddih.h](lib/he/ddih.h)
: derivatives of vector edges, triagels, dihidrals

## Utility

[array.h](lib/he/array.h)
:  array related functions

[err.h](lib/he/err.h)
:   error handling

[macro.h](lib/he/macro.h)
:   macros

[memory.h](lib/he/memory.h)
:   memory related

[util.h](lib/he/util.h)
:   uncategorazed

## Surface properties

[area.h](lib/he/area.h)
:   area

[volume.h](lib/he/volume.h)
:   volume

[laplace.h](lib/he/laplace.h)
:   Laplace operator of coordinates

[normal.h](lib/he/normal.h)
:   normal

## Surface transformation

[equiangulate.h](lib/he/equiangulate.h)
:   equlatirate triangles

[orient.h](lib/he/orient.h)
:   orient surface in a direction of eigen values of momentum tensor

[restore.h](lib/he/restore.h)
:  restore a volume of the surface

## Half-edg related

[read.h](lib/he/read.h)
:   read half-edg to intermediate structure HeRead, used to initialize
    **he.h**

[he.h](lib/he/he.h)
:   half edg data structure (provides half-edg API)

[hash.h](lib/he/hash.h)
:   stores an integer for a pair of integers

## IO

[off.h](lib/he/off.h), [punto.h](lib/he/punto.h), [vtk.h](lib/he/vtk.h), [gts.h](lib/he/gts.h), [ply.h](lib/he/ply.h), [obj.h](lib/he/obj.h)
:   read/write off files, punto, vtk, gts, ply, obj files

## X and Y

[x.h](lib/he/x.h)
:   simple interface for one surface

[y.h](lib/he/y.h)
:   not so simple interface
