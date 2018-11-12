D = \
tool\
pkgconfig\
lib\
bin/geomview\
bin/shake\
bin/maxima\
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
