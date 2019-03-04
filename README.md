
![red cell](img/rbc/sde/Da-0.358.png)\

# Introduction
Corpuscles is a C library to resolve the elastic energy and force
of a membrane surface, which is represented by an unstructured triangulated mesh.
It also simulates transformations between different configurations/shapes
of a closed membrane by a minimization procedure
under various constraints.

Various components can be switched on/off at will, such as
A. Bending elasticity of lipip bilayer.
B. Strain (both shear and strecth) easlticity of cytoskeleton.
C. Viscosity of lipid membrane.
D. Thermal fluctuations of lipid membrane.
E. Constraint of total area of a closed membrane surface.
F. Constratin of total volume of a closed membrane surface.
G.  Constratin of area difference between outer- and inner-surface.

For different components, variations of model/law can be selected
such as Helfrich/spontaneous curvature model for bending elasticity
and Skalak law for strain elasticity.

For different models/laws, variations of schemes impelemented
can also be choosen such as Gompper&Kroll scheme
and Juelicher schemes for the bending model of spontaneous curvature.

Please see details in user_manual.pdf[to be created]

# Install
Minimal requirements `pkg-config`, `make`, `gcc-c`. Optional
requrements `gsl`, `geomview`, `atest`, `maxima`.

Download code from
```sh
$ git@github.com:cselab/corpuscles.git
```

Adjust `conf.mk` if you want to change defaults

```makefile
CC = c99
CFLAGS = -O2 -g
PREFIX = $(HOME)
DATAPATH = $(HOME)/.co
MAXIMA_HOME = $(HOME)/.maxima

# prefix
P = co
```

Library is installed under the path `PREFIX`. Add `PREFIX/bin` and
`PREFIX/man` to envariament variables `PATH` and `MANPATH`. Run
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
#include <co/tri.h>

#define FMT CO_REAL_OUT

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
CO_CFLAGS = `co.conf --cflags $(PREC)`
CO_LDFLAGS = `co.conf --libs $(PREC)`

main: main.c
	$(CC) main.c $(CFLAGS) $(CO_CFLAGS) $(LDFLAGS) $(CO_LDFLAGS) -o main
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

#include <real.h>
#include <co/array.h>
#include <co/he.h>
#include <co/y.h>

#define FMT CO_REAL_OUT

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

### write off file

Read off, compute area fro every triangle, and output off file with colors ([example/off/write/area](example/off/write/area/))

```c
#include <stdio.h>

#include <real.h>
#include <co/area.h>
#include <co/err.h>
#include <co/off.h>
#include <co/he.h>
#include <co/memory.h>
#include <co/y.h>

int main(void) {
    int nt;
    real *x, *y, *z, *a;
    He *he;
    y_inif(stdin, &he, &x, &y, &z);
    nt = he_nt(he);
    CALLOC(nt, &a);
    he_area_tri(he, x, y, z, a);
    boff_tri_fwrite(he, x, y, z, a, stdout);
    y_fin(he, x, y, z);
    FREE(a);
}

```
```sh
./main < $(co.path)/rbc/laplace/0.off > out.off
```

```sh
co.geomview -r 55.9195 -13.672 8.69021 -f 25.0389 out.off
```

![mesh colored by triangle area](img/area.png)\

# Visualization

We use a wrapper to [geomview](http://geomview.org).

```sh
$ co.geomview -h
co.geomview [-t x y z] [-r x y z] [-a APPEARANCE] [-o FILE] [OFF]..
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
co.geomview -t 0.25 0.25 0     data/rbc.off
co.geomview -a data/appearance data/rbc.off
co.geomview -o snap.ppm        data/rbc.off
co.geomview                    data/rbc.off data/sph.off
co.geomview -p co.orient       data/rbc.off data/sph.off
co.geomview -c off.volume      data/rbc.off data/sph.off

```

# Lib

## Floating point precision

[prec/s/real.h](lib/co/prec/s/real.h), [prec/d/real.h](lib/co/prec/d/real.h), [prec/l/real.h](lib/co/prec/l/real.h)
:   single, double, long double

## Math

[vec.h](lib/co/vec.h), [edg.h](lib/co/edg.h), [tri.h](lib/co/tri.h), [dih.h](lib/co/dih.h), [ten.h](lib/co/ten.h)
: vector, edges, triangels, dihidrals, tensors

[dvec.h](lib/co/dvec.h), [dedg.h](lib/co/dedg.h), [dtri.h](lib/co/dtri.h), [ddih.h](lib/co/ddih.h)
: derivatives of vector edges, triagels, dihidrals

[ring.h](lib/co/ring.h)
: operation on the first ring of neighbors

## Utility

[array.h](lib/co/array.h)
:  array related functions

[argv.h](lib/co/argv.h)
:  read from `argv` and shift

[err.h](lib/co/err.h)
:   error handling

r_enddian(endian.h)
:   deal with endianess

[macro.h](lib/co/macro.h)
:   macros

r_sum(sum.h)
:   Kahan summation

[memory.h](lib/co/memory.h)
:   memory related

[util.h](lib/co/util.h)
:   uncategorazed

## Surface properties

[area.h](lib/co/area.h)
:   area

[volume.h](lib/co/volume.h)
:   volume

[laplace.h](lib/co/laplace.h)
:   Laplace operator of coordinates

[normal.h](lib/co/normal.h)
:   normal

## "Forces"

[bending.h](lib/co/bending.h)
:   generic interface to several bending forces

[forces.h](lib/co/forces.h)
:   generic interface to forces

[stretch.h](lib/co/stretch.h)
:   stretching force

## Surface transformation

[equiangulate.h](lib/co/equiangulate.h)
:   equlatirate triangles

[orient.h](lib/co/orient.h)
:   orient surface in a direction of eigen values of momentum tensor

[restore.h](lib/co/restore.h)
:  restore a volume of the surface

[transformation.h](lib/co/transformation.h)
:  translate, rotate, and and scale surface

## Half-edg related

[read.h](lib/co/read.h)
:   read half-edg to intermediate structure HeRead, used to initialize half-edg

[he.h](lib/co/he.h)
:   half edg data structure (provides half-edg API)

[hash.h](lib/co/hash.h)
:   stores an integer for a pair of integers

## IO

[off.h](lib/co/off.h), [punto.h](lib/co/punto.h), [vtk.h](lib/co/vtk.h), [gts.h](lib/co/gts.h), [ply.h](lib/co/ply.h), [obj.h](lib/co/obj.h)
:   read/write off files, punto, vtk, gts, ply, obj files

## X and Y

[x.h](lib/co/x.h)
:   simple interface for one surface

[y.h](lib/co/y.h)
:   not so simple interface

# Documentation

[git pages](https://cselab.github.io/corpuscles)
[docs/index.html](docs/index.html) is generated from
[README.md.m4](README.md.m4). To update run

``` sh
make html
```

requires [pandoc](http://pandoc.org).
