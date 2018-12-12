S = ../..
include  $S/conf.mk

B = $(PREFIX)/bin
I = $(PREFIX)/include

P = he

M = m4

include $S/make/tool.mk

$M: $M.in
	sed 's,%include%,$I,g' $M.in > $M
	chmod a+x $M

test:
clean:; rm -f conf

.PHONY: clean install test
