S = ../../..
include $S/make/html.mk

M = index.html
all: $M

$M: webgl.js matrix.js webgl.css

run: $M; firefox index.html
.PHONY: clean all run
clean:; rm -f $M
