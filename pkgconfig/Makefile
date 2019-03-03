include ../conf.mk
TRG = he
P = he_s.pc he_d.pc he_l.pc
SED = sed

install: $P
	mkdir -p -- "$(PREFIX)/share/pkgconfig"
	cp $P "$(PREFIX)/share/pkgconfig"

he_s.pc: $(TRG).pc.in
	$(SED) "s,@PREFIX@,$(PREFIX),g; s,@PREC@,s,g" $< > $@
he_d.pc: $(TRG).pc.in
	$(SED) "s,@PREFIX@,$(PREFIX),g; s,@PREC@,d,g" $< > $@
he_l.pc: $(TRG).pc.in
	$(SED) "s,@PREFIX@,$(PREFIX),g; s,@PREC@,l,g" $< > $@

clean:; rm -f $P
test:

.PHONY: clean install test
