include ../conf.mk

S = ..

B = $(PREFIX)/bin
P = he

M = conf dep util install

include $S/make/tool.mk
conf: conf.in
	sed "s,@PREFIX@,$(PREFIX),g" $< > $@
	chmod +x $@

test: install; atest test/*

clean:; rm -f conf

.PHONY: clean install test
