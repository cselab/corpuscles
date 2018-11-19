D = \
tool\
pkgconfig\
lib\
bin/geomview\
bin/ffmpeg\
bin/shake\
bin/orient\
bin/maxima\
bin/path\
example/area/tri\
example/area/voronoi\
example/he\
example/punto\
example/ten\
example/valid\
example/vec\
example/volume\
data

install: dir
test: dir
clean: dir
include make/dir.mk

.PHONY: test install clean
