.SUFFIXES:
.SUFFIXES: .pc.in _s.pc _d.pc _l.pc
include ../conf.mk
PC = co_s.pc co_d.pc co_l.pc alg.pc.in fm.pc.in
SED = sed

install: $(PC)
	mkdir -p -- "$(PREFIX)/share/pkgconfig"
	cp $(PC) "$(PREFIX)/share/pkgconfig"

_s.pc:; $(SED) "s,@PREFIX@,$(PREFIX),g; s,@PREC@,s,g" $< > $@
# co_d.pc: $P.pc.in
# 	$(SED) "s,@PREFIX@,$(PREFIX),g; s,@PREC@,d,g" $< > $@
# co_l.pc: $P.pc.in
# 	$(SED) "s,@PREFIX@,$(PREFIX),g; s,@PREC@,l,g" $< > $@

clean:; rm -f $(PC)
test:

.PHONY: clean install test
