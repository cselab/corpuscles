include(`co.m4')dnl
h_include(`m4/util.m4')dnl
r_geomview(dnl
<<-p 'co.q al img/rbc/ref.off <' -t -0.06 -0.013 0 -r -64 -32 9.5 -f 25 img/rbc/sde/Da-0.358.off>>,
<<img/rbc/sde/Da-0.358.png>>,
red cell)
# Introduction
Corpuscles is a C library to resolve the elastic energy and force of a
membrane surface, which is represented by an unstructured triangulated
mesh.  It also simulates transformations between different
configurations/shapes of a closed membrane by a minimization procedure
under various constraints.

Various components can be switched on/off at will, such as<br>
A. Bending elasticity of lipip bilayer.<br>
B. Strain (both shear and strecth) elasticity of cytoskeleton.<br>
C. Viscosity of lipid membrane.<br>
D. Thermal fluctuations of lipid membrane.<br>
E. Constraint of reduced volume of a closed membrane surface.<br>
F. Constraint of total area of a closed membrane surface.<br>
G. Constratin of total volume of a closed membrane surface.<br>
H. Constratin of area difference between outer- and inner-surface.<br>

For different components, variations of model/law can be selected such
as Helfrich/spontaneous curvature model for bending elasticity and
Skalak law for strain elasticity.

For different models/laws, variations of schemes impelemented can also
be choosen such as Gompper&Kroll scheme and Juelicher scheme for the
bending model of spontaneous curvature.

Please see details in r_link(https://cselab.github.io/corpuscles)

# Install
Minimal requirements `pkg-config`, `make`, `gcc-c`. Optional
requrements `gsl`, `geomview`, `atest`, `maxima`, `pandoc`.

Download code from
```sh
$ git@github.com:cselab/corpuscles.git
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

## hello world

A simple example is in r_dir(example/hello)

```c
$ cat main.c
r_file(example/hello/main.c)
```

```make
$ cat Makefile
r_file(example/hello/Makefile)
```

## read off file

An example is in r_dir(example/off/read)

```c
$ cat main.c
r_file(example/off/read/main.c)
```

## write off file

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
<<img/area.png>>,
mesh colored by triangle area)dnl
\

# Visualization

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

r_header(ring.h)
: operation on the first ring of neighbors

## Utility

r_header(array.h)
:  array related functions

r_header(argv.h)
:  read from `argv` and shift

r_header(err.h)
:   error handling

r_header(endian.h)
:   deal with endianess

r_header(macro.h)
:   macros

r_header(sum.h)
:   [Kahan summation](en.wikipedia.org/wiki/Kahan_summation_algorithm)

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

# Publication

Bian, X., Litvinov, S., & Koumoutsakos, P. (2020). Bending models of
lipid bilayer membranes: Spontaneous curvature and area-difference
elasticity. Computer Methods in Applied Mechanics and Engineering,
359, 112758. r_link(https://doi.org/10.1016/j.cma.2019.112758)
