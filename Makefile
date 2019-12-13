include conf.mk
PANDOC = pandoc

D = \
tool\
pkgconfig\
bin/m4\
m4/lib\
lib\
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
bin/q\
bin/run\
bin/shake\
bin/vtk\
bin/xcolor\
data\
example/area/tri\
example/area/voronoi\
example/he/dump\
example/punto\
example/ten\
example/valid\
example/vec\
example/volume\
maxima/lib

E = \
bin/align\
bin/cmoment\
bin/orient

install: show  dir
test: dir
clean: dir

lib: bin/m4 m4/lib

include make/dir.mk

README.md: m4/util.m4
html: README.md
	$(PANDOC) --css=css/swiss.css --standalone README.md --resource-path=docs --metadata pagetitle=corpuscles --output=docs/index.html

.PHONY: tool bin/m4 m4/lib lib test install clean show

.md.m4.md:; co.m4 -o $@ $<
.SUFFIXES: .md.m4 .md
