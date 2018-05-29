PREFIX = $(HOME)
S = ..

B = $(PREFIX)/bin
P = he

M = conf util
L = $(PREFIX)/lib/he

include $S/make/tool.mk

lib: lib.in
	sed "s,@LIB@,$L,g" $< > $@
	chmod +x $@

conf: conf.in
	sed "s,@PREFIX@,$(PREFIX),g" $< > $@
	chmod +x $@

clean:; rm -f conf
