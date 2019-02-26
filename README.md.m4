include(`he.m4')dnl
changequote(`[[', `]]')dnl
h_define([[r_header]], [[[$1](lib/he/$1)]])dnl
h_define([[r_file]], [[syscmd(cat '[[$1]]')]])dnl
h_define([[r_dir]], [[[$1]($1/)]])dnl
# Intro
![Intro image](img/rbc/sde/00006.png)

A C library for the modelling of triangulated surfaces shaped by forces
and constraints.

# Install

Minimal requirements `pkg-config`, `make`, `gcc-c`. Optional
requrements `gsl`, `geomview`, `atest`, `maxima`.

Download code from
```
$ git clone git@gitlab.ethz.ch:mavt-cse/he
```

Adjust `conf.mk` if you want to change defaults

```
r_file(conf.mk)dnl
```

Library is installed under the path `PREFIX`. Add `PREFIX/bin` and
`PREFIX/man` to envariament variables `PATH` and `MANPATH`. Run
`make`.

Run
```
$ make
```

If you have `atest` installed you can run
```
$ make test
```

# Examples

### hello world

A simple example is in r_dir(example/hello)

```
$ cat main.c
r_file(example/hello/main.c)
```

```
$ cat Makefile
r_file(example/hello/Makefile)
```

### read off file

An example is in r_dir(example/off/read)

```
$ cat main.c
r_file(example/off/read/main.c)
```

# visualization

We use a wrapper to [geomview](http://geomview.org).

```
$ he.geomview -h
syscmd([[he.geomview -h]])
```

# Lib

## Precision

[prec/d/real.h](lib/prec/d/real.h)
:   double

prec/s/real.h
:   single

prec/l/real.h
:   long double

## Math

edg.h, dedg.h
:   edges and derivatives

tri.h, dtri.h
:   triangles and derivatives

ten.h
:   tensor

vec.h, dvec.h
:   vectors and derivatives

## Utility

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

## Surface properties

area.h
:   area

volume.h
:   volume

laplace.h
:   Laplace operator of coordinates

normal.h
:   normal

## Surface transformation

equiangulate.h
:   equlatirate triangles

orient.h
:   orient surface in a direction of eigen values of momentum tensor

## Half-edg related

read.h
:   read half-edg to intermediate structure HeRead, used to initialize
	**he.h**

he.h
:   main half edg data structure (provides half-edg API)

hash.h
:   stores `v` for a pair of `(i, j)`, `i` must be in `[0, n)` API:
	`int i, j, k;` `ini(n); v ` set(i, j); get(i,j)= `get(i,j)` return
	-1 if `(i,j)` was not set

## IO

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

## X and Y

x.h
:   simple interface for one surface

y.h
:   not so simple interface
