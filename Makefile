include conf.mk
PANDOC = pandoc

D = \
tool\
pkgconfig\
m4/lib\
lib\
bin/2obj\
bin/2stl\
bin/2vtk\
bin/2xdmf\
bin/app\
bin/bead\
bin/eigenvector\
bin/eng\
bin/ffmpeg\
bin/geomview\
bin/host\
bin/list\
bin/loop\
bin/maxima\
bin/msh\
bin/obj\
bin/path\
bin/ply2off\
bin/q\
bin/run\
bin/shake\
bin/sqrt3\
bin/tile\
bin/upsample\
bin/vtk\
bin/wall\
bin/xcolor\
data\
example/he/dump\
example/punto\
example/ten\
example/vec\
maxima/lib\

E = \
bin/align\
bin/cmoment\
bin/orient\

install: dir
test: dir
clean: dir
	-rm TAGS

lib: bin/m4 m4/lib

include make/dir.mk
TAGS:; etags `find lib -name '*.c' -or -name '*.h' -or -name '*.m4' -or -name '*.inc'`

README.md: m4/util.m4
html: README.md
	$(PANDOC) --css=css/empty.css --standalone README.md --resource-path=docs -V lang=en-US --metadata pagetitle='Corpuscles documentation' --output=docs/index.html

.PHONY: tool bin/m4 m4/lib lib test install clean

.md.m4.md:; co.m4 -o $@ $<
.SUFFIXES: .md.m4 .md
