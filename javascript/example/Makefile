S = ../..
include $S/make/js.mk
include $S/make/html.mk

M = area he off vec tri hello.html
all: $M

run: $M; firefox hello.html

.PHONY: clean all run
clean:; rm -f $M