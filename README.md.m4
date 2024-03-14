include(`co.m4')dnl
h_include(`m4/util.m4')dnl
r_geomview(dnl
<<-p 'co.q al img/rbc/ref.off <' -t -0.06 -0.013 0 -r -64 -32 9.5 -f 25 img/rbc/sde/Da-0.358.off>>,
<<img/rbc/sde/Da-0.358.png>>,
red cell)
<H1>Corpuscles</H1>
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
Minimal dependencies are C and Fortran 90 compilers,
<a href="https://www.gnu.org/software/gsl">GNU Scientific Library</a>,
and
<a href="https://www.freedesktop.org/wiki/Software/pkg-config">pkg-config</a>.

Optional dependencies
<a href="http://geomview.org">geomview</a>,
<a href="https://maxima.sourceforge.io">maxima</a>,
and
<a href="https://pandoc.org">pandoc</a>.

Adjust r_source(conf.mk) if you want to change defaults

<pre>
$ cat conf.mk
r_file(conf.mk)dnl
</pre>

<tt>PREFIX/bin</tt> should be on the path, for example

<pre>
$ cat $HOME/.bashrc
PATH="$HOME/.local/bin:$PATH"
</pre>

Library is installed under the path <tt>PREFIX</tt>. Run
<pre>
$ MAKEFLAGS=-j4 make
</pre>

If you have
<a href=https://github.io/slitvinov/atest>atest</a>
you can run
<pre>
$ make test
</pre>

You can also use a r_source2(Dockerfile,docker containter).


<H2>Examples</H2>

<H3>Hello world</H3>

A program in r_dir(example/hello) computes area of a triangle.

<pre>
$ cat main.c
r_file(example/hello/main.c)
</pre>

<pre>
$ cat Makefile
r_file(example/hello/Makefile)
</pre>

<H3>Read mesh file</H3>

Corpuscles reads
<a href="https://en.wikipedia.org/wiki/OFF_(file_format)">OFF files</a>.
Here is r_source2(data/regular/tetra.off,a file with triangular pyramid).

r_geomview(dnl
<<-r -45 -20 0 $(co.path)/regular/tetra.off>>,
<<img/tetra.png>>,
triangular pyramid)

This program reads an OFF file from an input stream:

<pre>
$ cat main.c
r_file(example/off/read/main.c)
</pre>

<pre>
$ make
$ ./main < `co.path`/regular/tetra.off
r_cmd(<<cd example/off/read && make main > /dev/null && ./main < $(co.path)/regular/tetra.off>>)
</pre>

<H3>Write mesh file</H3>

Read off, compute area of every triangle, and output off file with
colors (r_dir(example/off/write/area))

<pre>
$ cat main.c
r_file(example/off/write/area/main.c)
</pre>

<pre>
$ ./main < $(co.path)/rbc/laplace/0.off > out.off
</pre>

To see the results

<pre>
$ co.geomview -r 55.9195 -13.672 8.69021 -f 25.0389 out.off
</pre>
r_geomview(<<-r 55.9195 -13.672 8.69021 -f 25.0389 img/area.off>>,
<<img/area.png>>,
mesh colored by triangle area)

<H3>Refine mesh</H3>

Using sqrt(3)-subdivision scheme
<pre>
$ co.geomview -r 60 -40 10 -f 22 `co.path`/rbc/icosahedron/0.off
$ co.sqrt3 < `co.path`/rbc/icosahedron/0.off > a.off
$ co.geomview -r 60 -40 10 -f 22 a.off
</pre>

r_geomview(<<-r 60 -40 10 -f 22 $(co.path)/rbc/icosahedron/0.off>>,
<<img/0.png>>,
red cell with 1280 trianglles)

r_geomview(<<-r 60 -40 10 -f 22 img/1.off>>,
<<img/1.png>>,
red cell with 3840 trianglles)

<H2>Visualization</H2>
We use a wrapper to
<a href="http://geomview.org">geomview</a>.

<pre>
$ co.geomview -h
r_cmd(<<co.geomview -h | m4/cat>>)
</pre>

<H2>Library</H2>

<H3>Floating point precision</H3>

r_header(prec/s/real.h), r_header(prec/d/real.h), r_header(prec/l/real.h)
:   single, double, long double

<H3>Math</H3>

r_header(vec.h), r_header(edg.h), r_header(tri.h), r_header(dih.h), r_header(ten.h)
: vector, edges, triangels, dihidrals, tensors

r_header(dvec.h), r_header(dedg.h), r_header(dtri.h), r_header(ddih.h)
: derivatives of vector edges, triagels, dihidrals

r_header(ring.h)
: operation on the first ring of neighbors

<H3>Utility</H3>

r_header(array.h)
:  array related functions

r_header(argv.h)
:  read from <tt>argv</tt> and shift

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

<H3>Surface properties</H3>

r_header(area.h)
:   area

r_header(volume.h)
:   volume

r_header(laplace.h)
:   Laplace operator of coordinates

r_header(normal.h)
:   normal

<H3>"Forces"</H3>

r_header(bending.h)
:   generic interface to several bending forces

r_header(forces.h)
:   generic interface to forces

r_header(stretch.h)
:   stretching force

<H3>Surface transformation</H3>

r_header(equiangulate.h)
:   equlatirate triangles

r_header(orient.h)
:   orient surface in a direction of eigen values of momentum tensor

r_header(restore.h)
:  restore a volume of the surface

r_header(transformation.h)
:  translate, rotate, and and scale surface

r_header(remesh.h)
: "edit" surfaces

<H3>Half-edg related</H3>

r_header(read.h)
:   read half-edg to intermediate structure HeRead, used to initialize half-edg

r_header(he.h)
:   half edg data structure (provides half-edg API)

r_header(hash.h)
:   stores an integer for a pair of integers

<H3>IO</H3>

r_header(gts.h),
r_header(obj.h),
r_header(off.h),
r_header(ply.h),
r_header(punto.h),
r_header(stl.h),
r_header(stl.h),
r_header(xdmf.h)
:   read/write GTS, OBJ, OFF, PLY, punto, STL, VTK, and XDMF files.

<H3>X and Y</H3>

r_header(x.h)
:   simple interface for one surface

r_header(y.h)
:   not so simple interface

<H2>Documentation</H2>

[git pages](https://cselab.github.io/corpuscles)
[docs/index.html](docs/index.html) is generated from
[README.md.m4](README.md.m4). To update run

<pre>
$ make html
</pre>

requires [pandoc](http://pandoc.org).

<H2>Hacking</H2>

After adding new files to r_dir(lib) update depdendencies:

<pre>
$ (cd lib && ./bootstrap)
</pre>

<H2>Publications</H2>

Bian, X., Litvinov, S., & Koumoutsakos, P. (2020). Bending models of
lipid bilayer membranes: Spontaneous curvature and area-difference
elasticity. Computer Methods in Applied Mechanics and Engineering,
359, 112758.
<a href="https://doi.org/10.1016/j.cma.2019.112758">doi:10.1016/j.cma.2019.112758</a>
