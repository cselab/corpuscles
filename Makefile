D = \
tool\
pkgconfig\
lib\
bin/align\
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
example/volume\
m4/lib\
maxima/lib

install: dir
test: dir
clean: dir
include make/dir.mk

.PHONY: test install clean
