PREFIX = $(HOME)
S = ../..

B = $(PREFIX)/bin
P = he

M = gshell gshell0

include $S/conf.mk
include $S/make/tool.mk

test: install; atest test/*

.PHONY: clean install test
