<p align="center"><img src="img/rbc/sde/Da-0.358.png"/></p>

<H2>Introduction</H2>
Corpuscles is a C library to resolve the elastic energy and force of a
membrane surface, which is represented by an unstructured triangulated
mesh.  It also simulates transformations between different
configurations/shapes of a closed membrane by a minimization procedure
under various constraints.

Various components can be switched on/off at will, such as<br>
<ol>
<li>Bending elasticity of lipip bilayer</li>
<li>Strain (both shear and strecth) elasticity of cytoskeleton</li>
<li>Viscosity of lipid membrane</li>
<li>Thermal fluctuations of lipid membrane</li>
<li>Constraint of reduced volume of a closed membrane surface</li>
<li>Constraint of total area of a closed membrane surface</li>
<li>Constratin of total volume of a closed membrane surface</li>
<li>Constratin of area difference between outer- and inner-surface</li>
</ol>

For different components, variations of model/law can be selected such
as Helfrich/spontaneous curvature model for bending elasticity and
Skalak law for strain elasticity.

For different models/laws, variations of schemes impelemented can also
be choosen such as Gompper and Kroll scheme and Juelicher scheme for
the bending model of spontaneous curvature. See more details in
[https://cselab.github.io/corpuscles](https://cselab.github.io/corpuscles)

<H2>Install</H2>
Minimal dependencies are C and Force 90 compilers,
<a href="https://www.gnu.org/software/gsl">GNU Scientific Library</a>,
and
<a href="https://www.freedesktop.org/wiki/Software/pkg-config">pkg-config</a>.

Optional dependencies
<a href="http://geomview.org">geomview</a>,
<a href="https://maxima.sourceforge.io">maxima</a>,
and
<a href="https://pandoc.org">pandoc</a>.

Adjust [conf.mk](https://github.com/cselab/corpuscles/blob/master/lib/co/conf.mk) if you want to change defaults

<pre>
$ cat conf.mk
CC = gcc
FC = gfortran

FCFLAGS = -Ofast  -g
FXFLAGS = -fallow-argument-mismatch

GSL_CFLAGS = " `gsl-config --cflags `"
GSL_LDFLAGS = " `gsl-config --libs `"

CFLAGS = -Ofast  -g
LDFLAGS =
PREFIX = $(HOME)/.local
DATAPATH = $(HOME)/.co
MAXIMA_HOME = $(HOME)/.maxima

# prefix for commands
P = co
</pre>

`PREFIX/bin` should be on the path, for example

<pre>
$ cat $HOME/.bashrc
PATH="$HOME/.local/bin:$PATH"
</pre>

Library is installed under the path `PREFIX`. Run
<pre>
$ MAKEFLAGS=-j4 ./install.sh
</pre>

If you have
<a href=https://github.io/slitvinov/atest>atest</a>
you can run
<pre>
$ make test
</pre>

<H2>Examples</H2>

<H3>hello world</H3>

A simple example is in [example/hello](https://github.com/cselab/corpuscles/blob/master/example/hello/)

<pre>
$ cat main.c
#include &lt;stdio.h&gt;

#include &lt;real.h&gt;
#include &lt;co/tri.h&gt;

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

</pre>

<pre>
$ cat Makefile
include ../../conf.mk
PREC = d
CO_CFLAGS =  `co.conf --cflags $(PREC) `
CO_LDFLAGS =  `co.conf --libs $(PREC) `

main: main.c
	$(CC) main.c $(CFLAGS) $(CO_CFLAGS) $(LDFLAGS) $(CO_LDFLAGS) -o main

.PHONY: clean
clean:; rm -f main

</pre>

<H3>read off file</H3>

An example is in [example/off/read](https://github.com/cselab/corpuscles/blob/master/example/off/read/)

<pre>
$ cat main.c
#include &lt;stdio.h&gt;

#include &lt;real.h&gt;
#include &lt;co/array.h&gt;
#include &lt;co/he.h&gt;
#include &lt;co/y.h&gt;

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

</pre>


<H3>write off file</H3>

Read off, compute area fro every triangle, and output off file with colors ([example/off/write/area](https://github.com/cselab/corpuscles/blob/master/example/off/write/area/))

<pre>
$ cat main.c
#include &lt;stdio.h&gt;

#include &lt;real.h&gt;
#include &lt;co/area.h&gt;
#include &lt;co/err.h&gt;
#include &lt;co/off.h&gt;
#include &lt;co/he.h&gt;
#include &lt;co/memory.h&gt;
#include &lt;co/y.h&gt;

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

</pre>

<pre>
$ ./main < $(co.path)/rbc/laplace/0.off > out.off
</pre>

To see the results

<pre>
$ co.geomview -r 55.9195 -13.672 8.69021 -f 25.0389 out.off
</pre>
<p align="center"><img src="img/area.png"/></p>

<H2>Visualization</H2>
We use a wrapper to [geomview](http://geomview.org).

<pre>
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
-p command    process every off file by running  'command ' &lt; IN.off &gt; OUT.off
-n none|each|all|keep normalization status (see geomview manual)
-c command    run command on every file and write output to stderr, %f is replaced by a file name
-e command    if keys  'e ' is pressed runs
               '&lt;OFF command -t x y z -r x y z -f zoom -i [index of a file] -n  [number of files]  ' or
               '&lt;OFF command ' if -e  '|command ' is passed
-i command    run command on every image, %i replaced by input; %o -- by output; %b --- by basename

Keys:
    q: quit
    s: save snap.ppm
    S: save every snapshot
    e: run command on OFF file (see -e option)
    p: panel
    j/k: switch between off files
    J: dump file name
    [SPC]: dump orientation and field of view

Environment variables:
WX, WY: resolution of the snapshot (default: 800x600)
BACKGROUND: default ( '1 1 1 ')
GEOMVIEW_ARGS: pass to geomview

Examples:
co.geomview -t 0.25 0.25 0     data/rbc.off
co.geomview -a data/appearance data/rbc.off
co.geomview -o snap.ppm        data/rbc.off
co.geomview                    data/rbc.off data/sph.off
co.geomview -p co.orient       data/rbc.off data/sph.off
co.geomview -c off.volume      data/rbc.off data/sph.off

</pre>

<H2>Library</H2>

<H3>Floating point precision</H3>

[prec/s/real.h](https://github.com/cselab/corpuscles/blob/master/lib/co/prec/s/real.h), [prec/d/real.h](https://github.com/cselab/corpuscles/blob/master/lib/co/prec/d/real.h), [prec/l/real.h](https://github.com/cselab/corpuscles/blob/master/lib/co/prec/l/real.h)
:   single, double, long double

<H3>Math</H3>

[vec.h](https://github.com/cselab/corpuscles/blob/master/lib/co/vec.h), [edg.h](https://github.com/cselab/corpuscles/blob/master/lib/co/edg.h), [tri.h](https://github.com/cselab/corpuscles/blob/master/lib/co/tri.h), [dih.h](https://github.com/cselab/corpuscles/blob/master/lib/co/dih.h), [ten.h](https://github.com/cselab/corpuscles/blob/master/lib/co/ten.h)
: vector, edges, triangels, dihidrals, tensors

[dvec.h](https://github.com/cselab/corpuscles/blob/master/lib/co/dvec.h), [dedg.h](https://github.com/cselab/corpuscles/blob/master/lib/co/dedg.h), [dtri.h](https://github.com/cselab/corpuscles/blob/master/lib/co/dtri.h), [ddih.h](https://github.com/cselab/corpuscles/blob/master/lib/co/ddih.h)
: derivatives of vector edges, triagels, dihidrals

[ring.h](https://github.com/cselab/corpuscles/blob/master/lib/co/ring.h)
: operation on the first ring of neighbors

<H3>Utility</H3>

[array.h](https://github.com/cselab/corpuscles/blob/master/lib/co/array.h)
:  array related functions

[argv.h](https://github.com/cselab/corpuscles/blob/master/lib/co/argv.h)
:  read from `argv` and shift

[err.h](https://github.com/cselab/corpuscles/blob/master/lib/co/err.h)
:   error handling

[endian.h](https://github.com/cselab/corpuscles/blob/master/lib/co/endian.h)
:   deal with endianess

[macro.h](https://github.com/cselab/corpuscles/blob/master/lib/co/macro.h)
:   macros

[sum.h](https://github.com/cselab/corpuscles/blob/master/lib/co/sum.h)
:   [Kahan summation](en.wikipedia.org/wiki/Kahan_summation_algorithm)

[memory.h](https://github.com/cselab/corpuscles/blob/master/lib/co/memory.h)
:   memory related

[util.h](https://github.com/cselab/corpuscles/blob/master/lib/co/util.h)
:   uncategorazed

<H3>Surface properties</H3>

[area.h](https://github.com/cselab/corpuscles/blob/master/lib/co/area.h)
:   area

[volume.h](https://github.com/cselab/corpuscles/blob/master/lib/co/volume.h)
:   volume

[laplace.h](https://github.com/cselab/corpuscles/blob/master/lib/co/laplace.h)
:   Laplace operator of coordinates

[normal.h](https://github.com/cselab/corpuscles/blob/master/lib/co/normal.h)
:   normal

<H3>"Forces"</H3>

[bending.h](https://github.com/cselab/corpuscles/blob/master/lib/co/bending.h)
:   generic interface to several bending forces

[forces.h](https://github.com/cselab/corpuscles/blob/master/lib/co/forces.h)
:   generic interface to forces

[stretch.h](https://github.com/cselab/corpuscles/blob/master/lib/co/stretch.h)
:   stretching force

## Surface transformation

[equiangulate.h](https://github.com/cselab/corpuscles/blob/master/lib/co/equiangulate.h)
:   equlatirate triangles

[orient.h](https://github.com/cselab/corpuscles/blob/master/lib/co/orient.h)
:   orient surface in a direction of eigen values of momentum tensor

[restore.h](https://github.com/cselab/corpuscles/blob/master/lib/co/restore.h)
:  restore a volume of the surface

[transformation.h](https://github.com/cselab/corpuscles/blob/master/lib/co/transformation.h)
:  translate, rotate, and and scale surface

<H3>Half-edg related</H3>

[read.h](https://github.com/cselab/corpuscles/blob/master/lib/co/read.h)
:   read half-edg to intermediate structure HeRead, used to initialize half-edg

[he.h](https://github.com/cselab/corpuscles/blob/master/lib/co/he.h)
:   half edg data structure (provides half-edg API)

[hash.h](https://github.com/cselab/corpuscles/blob/master/lib/co/hash.h)
:   stores an integer for a pair of integers

<H3>IO</H3>

[off.h](https://github.com/cselab/corpuscles/blob/master/lib/co/off.h), [punto.h](https://github.com/cselab/corpuscles/blob/master/lib/co/punto.h), [vtk.h](https://github.com/cselab/corpuscles/blob/master/lib/co/vtk.h), [gts.h](https://github.com/cselab/corpuscles/blob/master/lib/co/gts.h), [ply.h](https://github.com/cselab/corpuscles/blob/master/lib/co/ply.h), [obj.h](https://github.com/cselab/corpuscles/blob/master/lib/co/obj.h)
:   read/write off files, punto, vtk, gts, ply, obj files

<H3>X and Y</H3>

[x.h](https://github.com/cselab/corpuscles/blob/master/lib/co/x.h)
:   simple interface for one surface

[y.h](https://github.com/cselab/corpuscles/blob/master/lib/co/y.h)
:   not so simple interface

<H2>Documentation</H2>

[git pages](https://cselab.github.io/corpuscles)
[docs/index.html](docs/index.html) is generated from
[README.md.m4](README.md.m4). To update run

<pre>
$ make html
</pre>

requires [pandoc](http://pandoc.org).

<H2>Publications</H2>

Bian, X., Litvinov, S., & Koumoutsakos, P. (2020). Bending models of
lipid bilayer membranes: Spontaneous curvature and area-difference
elasticity. Computer Methods in Applied Mechanics and Engineering,
359, 112758.
<a href="https://doi.org/10.1016/j.cma.2019.112758">doi:10.1016/j.cma.2019.112758</a>
