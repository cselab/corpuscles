S = ../..
include $S/conf.mk

PREFIX = $(HOME)
B = $(PREFIX)/bin
P = he

M = maxima
MAXIMA_HOME = $(HOME)/.maxima

include $S/make/tool.mk

maxima: maxima.in
	sed "s,@MAXIMA_HOME@,$(MAXIMA_HOME),g" $< > $@
	chmod a+x $@


conf:; he.install batch.lisp $(MAXIMA_HOME)/he
install: conf
test: install conf; atest test/*

.PHONY: clean install test
