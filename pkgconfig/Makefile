PREFIX = $(HOME)
TRG = he
P = he_s.pc he_d.pc
SED = sed

install: $P; cp $P $(PREFIX)/share/pkgconfig

he_s.pc: $(TRG).pc.in
	$(SED) "s,@PREFIX@,$(PREFIX),g; s,@PREC@,s,g" $< > $@
he_d.pc: $(TRG).pc.in
	$(SED) "s,@PREFIX@,$(PREFIX),g; s,@PREC@,d,g" $< > $@

clean:; rm -f $P
test:

.PHONY: clean install test
