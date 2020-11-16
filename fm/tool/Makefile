S = ..
include $S/conf.mk

B = $(PREFIX)/bin
P = fm

M = conf util

include $S/make/tool.mk
conf: conf.in
	sed "s,@PREFIX@,$(PREFIX),g" $< > $@
	chmod +x $@

test: install

clean:; rm -f conf

.PHONY: clean install test
