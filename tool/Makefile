S = ..
include $S/conf.mk
B = $(PREFIX)/bin
I = $(PREFIX)/include
M = \
conf\
dep\
install\
m4\
util\

include $S/make/tool.mk
conf: conf.in
	sed "s,@PREFIX@,$(PREFIX),g" $< > $@
	chmod +x $@

m4: m4.in
	sed 's,%include%,$I,g' $< > $@
	chmod a+x $M

test: install; atest test/*

clean:; rm -f conf m4

.PHONY: clean install test
