S = ../..
include $S/conf.mk

B = $(PREFIX)/bin
P = he

M = ffmpeg

include $S/make/tool.mk

test: install; atest test/*

clean:; rm -f conf

.PHONY: clean install test
