Intro
=====

[Intro image](img/rbc/sde/00006.png)

A library for the modelling of surfaces shaped by forces and
constraints.


Install
=======

Minimal requirements `pkg-config`, `make`, `gcc-c`. Optional
requrements `gsl`, `geomview`, `atest`.

Download code from
```
git clone git@gitlab.ethz.ch:mavt-cse/he
```

Library is installed under directory `PREFIX`

Add `PREFIX/bin` to `PATH` and `PREFIX/man` to `MANPATH`.

```
make PREFIX=$HOME/he
```

c compiler and flags can be set, for example

```
make CC=clang CFLAGS='-Wall -Wextra -g -O2'
```

Lib
===

Precision
---------

prec/d/real.h
:   double

prec/s/real.h
:   single

prec/l/real.h
:   long double

Math
----

edg.h, dedg.h
:   edges and derivatives

tri.h, dtri.h
:   triangles and derivatives

ten.h
:   tensor

vec.h, dvec.h
:   vectors and derivatives

Utility
-------

array.h
:   array related functions

err.h
:   error handling

macro.h
:   macros

memory.h
:   memory related

util.h
:   uncategorazed

Mesh properties
---------------

area.h
:   area

volume.h
:   volume

laplace.h
:   Laplace operator of coordinates

normal.h
:   normal

Mesh transformation
-------------------

equiangulate.h
:   equlatirate triangles

orient.h
:   orient mesh in a direction of eigen values of momentum tensor

Half-edg related
----------------

read.h
:   read half-edg to intermediate structure HeRead, used to initialize
	**he.h**

he.h
:   main half edg data structure (provides half-edg API)

hash.h
:   stores `v` for a pair of `(i, j)`, `i` must be in `[0, n)` API:
	`int i, j, k;` `ini(n); v ` set(i, j); get(i,j)= `get(i,j)` return
	-1 if `(i,j)` was not set

IO
--

off.h
:   read OFF

punto.h
:   read/write "punto" files

vtk.h
:   write vtk file

gts.h
:   write gts files

ply.h
:   read/write ply files

obj.h
:   read/write obj files

X
-

x.h
:   simplifyied interface for one mesh

y.h
:   less simplifyied interface mesh
