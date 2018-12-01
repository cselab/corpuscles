S = ../../..

all: gompper_xin.h gompper_xin.c

gompper_xin.h: gompper_xin.tmpl.h; ./force $^ > $@
gompper_xin.c: gompper_xin.tmpl.c; ./force $^ > $@

diff: all
	diff gompper_xin.h ref.h
	diff gompper_xin.c ref.c

clean:; rm -f gompper_xin.h gompper_xin.c

.PHONY: clean diff all
