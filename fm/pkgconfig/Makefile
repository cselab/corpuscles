include ../conf.mk
TRG = fm
P = fm_s.pc fm_d.pc fm_l.pc
SED = sed

install: $P
	mkdir -p -- "$(PREFIX)/share/pkgconfig"
	cp $P "$(PREFIX)/share/pkgconfig"

fm_s.pc: $(TRG).pc.in
	$(SED) "s,@PREFIX@,$(PREFIX),g; s,@PREC@,s,g" $< > $@

fm_d.pc: $(TRG).pc.in
	$(SED) "s,@PREFIX@,$(PREFIX),g; s,@PREC@,d,g" $< > $@

fm_l.pc: $(TRG).pc.in
	$(SED) "s,@PREFIX@,$(PREFIX),g; s,@PREC@,l,g" $< > $@

clean:; rm -f $P
test:

.PHONY: clean install test
