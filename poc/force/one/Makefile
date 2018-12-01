S = ../../..

all: force.h force.c

force.h: tmpl.h 
	./force $^ > $@

force.c: tmpl.c
	./force $^ > $@

clean:; rm force.h force.c

.PHONY: clean all
