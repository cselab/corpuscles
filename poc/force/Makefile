all: force.h force.c

force.h: conf force.tmpl.h
	./force conf force.tmpl.h > force.h

force.c: conf force.tmpl.c
	./force conf force.tmpl.c > force.c
