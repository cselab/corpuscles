D = \
tool\
pkgconfig\
bin/m4\
m4/lib\
lib\
bin/align\
bin/cmoment\
bin/ffmpeg\
bin/geomview\
bin/maxima\
bin/orient\
bin/path\
bin/q\
bin/run\
bin/shake\
bin/vtk\
data\
example/area/tri\
example/area/voronoi\
example/he\
example/punto\
example/ten\
example/valid\
example/vec\
example/volume\
maxima/lib

install: dir
test: dir
clean: dir

lib: bin/m4 m4/lib

include make/dir.mk

.PHONY: tool bin/m4 m4/lib lib test install clean
