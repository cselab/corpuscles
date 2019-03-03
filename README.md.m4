include(`co.m4')dnl
h_define(`r_define',
`h_define($1, `h_changequote()h_changequote(`,')`'$2`'h_changequote(`<<',`>>')')')dnl
r_define(`r_header', `[$1](lib/co/$1)')dnl
r_define(`r_file', `h_cmd(cat $1)')dnl
r_define(`r_cmd', `h_cmd($1)')dnl
r_define(`r_dir', `[$1]($1/)')dnl
r_define(`r_geomview', `h_syscmd(WX=400 WY=300 co.geomview -o $2 -i "convert %i %o" $1)dnl
h_syscmd(co.install "$2" docs)
![]($2)')dnl
h_changequote(`<<', `>>')dnl
r_geomview(
<<-p 'co.q al img/rbc/ref.off <' -t -0.06 -0.013 0 -r -64 -32 9.5 -f 25 img/rbc/sde/Da-0.358.off>>,
<<img/rbc/sde/Da-0.358.png>>)dnl
\

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
r_file(conf.mk)dnl
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

A simple example is in r_dir(example/hello)

```c
$ cat main.c
r_file(example/hello/main.c)
```

```make
$ cat Makefile
r_file(example/hello/Makefile)
```

### read off file

An example is in r_dir(example/off/read)

```c
$ cat main.c
r_file(example/off/read/main.c)
```

### write off file

Read off, compute area fro every triangle, and output off file with colors (r_dir(example/off/write/area))

```c
r_file(example/off/write/area/main.c)
```
```sh
./main < $(co.path)/rbc/laplace/0.off > out.off
```

```sh
co.geomview -r 55.9195 -13.672 8.69021 -f 25.0389 out.off
```
r_geomview(<<-r 55.9195 -13.672 8.69021 -f 25.0389 img/area.off>>,
<<img/area.png>>)dnl
\

# visualization

We use a wrapper to [geomview](http://geomview.org).

```sh
$ co.geomview -h
r_cmd(<<co.geomview -h>>)
```

# Lib

## Floating point precision

r_header(prec/s/real.h), r_header(prec/d/real.h), r_header(prec/l/real.h)
:   single, double, long double

## Math

r_header(vec.h), r_header(edg.h), r_header(tri.h), r_header(dih.h), r_header(ten.h)
: vector, edges, triangels, dihidrals, tensors

r_header(dvec.h), r_header(dedg.h), r_header(dtri.h), r_header(ddih.h)
: derivatives of vector edges, triagels, dihidrals


## Utility

r_header(array.h)
:  array related functions

r_header(argv.h)
:  read from `argv` and shift

r_header(err.h)
:   error handling

r_enddian(endian.h)
:   deal with endianess

r_header(macro.h)
:   macros

r_sum(sum.h)
:   Kahan summation

r_header(memory.h)
:   memory related

r_header(util.h)
:   uncategorazed

## Surface properties

r_header(area.h)
:   area

r_header(volume.h)
:   volume

r_header(laplace.h)
:   Laplace operator of coordinates

r_header(normal.h)
:   normal

## "Forces"

r_header(bending.h)
:   generic interface to several bending forces

r_header(forces.h)
:   generic interface to forces

r_header(stretch.h)
:   stretching force

## Surface transformation

r_header(equiangulate.h)
:   equlatirate triangles

r_header(orient.h)
:   orient surface in a direction of eigen values of momentum tensor

r_header(restore.h)
:  restore a volume of the surface

r_header(transformation.h)
:  translate, rotate, and and scale surface

## Half-edg related

r_header(read.h)
:   read half-edg to intermediate structure HeRead, used to initialize half-edg

r_header(he.h)
:   half edg data structure (provides half-edg API)

r_header(hash.h)
:   stores an integer for a pair of integers

## IO

r_header(off.h), r_header(punto.h), r_header(vtk.h), r_header(gts.h), r_header(ply.h), r_header(obj.h)
:   read/write off files, punto, vtk, gts, ply, obj files

## X and Y

r_header(x.h)
:   simple interface for one surface

r_header(y.h)
:   not so simple interface

# Documentation

[git pages](https://cselab.github.io/corpuscles)
[docs/index.html](docs/index.html) is generated from
[README.md.m4](README.md.m4). To update run

``` sh
make html
```

requires [pandoc](http://pandoc.org).
