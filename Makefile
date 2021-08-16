include conf.mk
PANDOC = pandoc

D = \
tool\
pkgconfig\
m4/lib\
lib\
bin/app\
bin/bead\
bin/2vtk\
bin/eigenvector\
bin/eng\
bin/ffmpeg\
bin/geomview\
bin/host\
bin/loop\
bin/maxima\
bin/msh\
bin/path\
bin/tile\
bin/q\
bin/obj\
bin/list\
bin/run\
bin/ply2off\
bin/shake\
bin/sqrt3\
bin/vtk\
bin/xcolor\
bin/upsample\
bin/wall\
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
