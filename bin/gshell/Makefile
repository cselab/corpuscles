S = ../..
include $S/conf.mk

B = $(PREFIX)/bin
M = gshell gshell0

include $S/conf.mk
include $S/make/tool.mk

test: install; atest test/*

.PHONY: clean install test
