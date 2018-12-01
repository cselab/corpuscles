S = ../..

all: force.h force.c

force.h: conf force.tmpl.h
	./force conf $< > $@

force.c: conf force.tmpl.c
	./force conf $< > $@

install: force.h force.c
	cp force.h $S/lib/he/force.h
	cp force.c $S/lib/force.c

clean:; rm force.h force.c


.PHONY: clean install all
