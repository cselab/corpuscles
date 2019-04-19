S = ../../..
include $S/make/html.mk

M = index.html
all: $M

$M: webgl.js

run: $M; firefox index.html
.PHONY: clean all run
clean:; rm -f $M
