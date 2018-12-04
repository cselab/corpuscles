S = ../../..
M = \
gompper_xin.h\
gompper_xin.c\
area_voronoi.h\
area_voronoi.c\
garea_voronoi.h\
garea_voronoi.c\
volume_normal.h\
volume_normal.c

all: $M

gompper_xin.h: gompper_xin.tmpl.h; ./force $^ > $@
gompper_xin.c: gompper_xin.tmpl.c; ./force $^ > $@

area_voronoi.h: area_voronoi.tmpl.h; ./force $^ > $@
area_voronoi.c: area_voronoi.tmpl.c; ./force $^ > $@

garea_voronoi.h: garea_voronoi.tmpl.h; ./force $^ > $@
garea_voronoi.c: garea_voronoi.tmpl.c; ./force $^ > $@

volume_normal.h: volume_normal.tmpl.h; ./force $^ > $@
volume_normal.c: volume_normal.tmpl.c; ./force $^ > $@

diff: all
	diff gompper_xin.h ref.h
	diff gompper_xin.c ref.c

install: all
	cp area_voronoi.h $S/lib/he/f
	cp area_voronoi.c $S/lib/f
	cp garea_voronoi.h $S/lib/he/f
	cp garea_voronoi.c $S/lib/f
	cp volume_normal.h $S/lib/he/f
	cp volume_normal.c $S/lib/f

clean:; rm -f $M

.PHONY: clean diff all install
