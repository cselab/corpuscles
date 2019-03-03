S = ../..
include $S/conf.mk

B = $(PREFIX)/bin
M = geomview geomview0

include $S/conf.mk
include $S/make/tool.mk

geomview0: geomview0.m4 args.m4 util.m4; co.m4 $< > $@
geomview:  geomview.m4 args.m4 util.m4; co.m4 $< > $@

test: install; atest test/*
clean:; rm -f geomview geomview0

.PHONY: clean install test
