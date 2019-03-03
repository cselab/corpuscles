S = ../..
include  $S/conf.mk

B = $(PREFIX)/bin
M = path

include $S/make/tool.mk

$M: $M.in
	sed 's,%path%,$(DATAPATH),g' $< > $@
	chmod a+x $@

test:
clean:; rm -f conf

.PHONY: clean install test
