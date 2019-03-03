S = ../..
include $S/conf.mk

B = $(PREFIX)/bin
M = shake

include $S/make/tool.mk

test: install; atest test/*

clean:; rm -f conf

.PHONY: clean install test
