S = ../..
include $S/make/html.mk

M = index.html
all: $M

run: $M; firefox index.html
.PHONY: clean all run
clean:; rm -f $M