S = ../..
include  $S/conf.mk

B = $(PREFIX)/bin

P = he

M = m4

path = $(HOME)/.he

include $S/make/tool.mk

$M: $M.in
	sed 's,%path%,$(path),g' $M.in > $M
	chmod a+x $M

test:
clean:; rm -f conf

.PHONY: clean install test
