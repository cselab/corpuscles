S = ../..
include $S/make/js.mk
include $S/make/html.mk

M = area he off0 vec tri hello.html
all: $M
hello.html: load.js

run: $M; firefox hello.html

.PHONY: clean all run
clean:; rm -f $M
