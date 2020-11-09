S = ../..
include $S/conf.mk

B = $(PREFIX)/bin
M = maxima
include $S/make/tool.mk

maxima: maxima.in
	sed "s,@MAXIMA_HOME@,$(MAXIMA_HOME),g" $< > $@
	chmod a+x $@


clean:; rm -f maxima
conf:; co.install batch.lisp $(MAXIMA_HOME)/he
install: conf
test: install conf; atest test/*

.PHONY: clean install test
