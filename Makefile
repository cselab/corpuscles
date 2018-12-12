D = \
tool\
pkgconfig\
lib\
bin/ffmpeg\
bin/geomview\
bin/m4\
bin/maxima\
bin/orient\
bin/path\
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
example/volume

install: dir
test: dir
clean: dir
include make/dir.mk

.PHONY: test install clean
