PREFIX = $(HOME)
S = ..

B = $(PREFIX)/bin
P = tri

M = awk lib conf util
L = $(PREFIX)/lib/tri

include $S/make/tool.mk

lib: lib.in
	sed "s,@LIB@,$L,g" $< > $@
	chmod +x $@

conf: conf.in
	sed "s,@PREFIX@,$(PREFIX),g" $< > $@
	chmod +x $@

clean:; rm -f lib
