S = ../../..
M = \
gompper_xin.h\
gompper_xin.c\
area_voronoi.h\
area_voronoi.c

all: $M

gompper_xin.h: gompper_xin.tmpl.h; ./force $^ > $@
gompper_xin.c: gompper_xin.tmpl.c; ./force $^ > $@

area_voronoi.h: area_voronoi.tmpl.h; ./force $^ > $@
area_voronoi.c: area_voronoi.tmpl.c; ./force $^ > $@

diff: all
	diff gompper_xin.h ref.h
	diff gompper_xin.c ref.c

install: all
	cp area_voronoi.h $S/lib/he/f
	cp area_voronoi.c $S/lib/f

clean:; rm -f $M

.PHONY: clean diff all install
