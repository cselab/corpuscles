PREFIX = $(HOME)
S = ../..

B = $(PREFIX)/bin
P = he

M = path

path = $(HOME)/.he

include $S/make/tool.mk

path: path.in
	sed 's,%path%,$(path),g' path.in > path
	chmod a+x path

test: install; atest test/*
clean:; rm -f conf

.PHONY: clean install test
