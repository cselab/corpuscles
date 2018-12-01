S = ../../..

all: force.h force.c

force.h: tmpl.h 
	./force $^ > $@

force.c: tmpl.c
	./force $^ > $@

diff: all
	diff force.h ref.h
	diff force.c ref.c

clean:; rm force.h force.c

.PHONY: clean diff all
