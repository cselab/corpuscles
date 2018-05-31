O_S = off.s.o x.s.o vec.s.o tri.s.o punto.s.o
O_D = off.d.o x.d.o vec.d.o tri.d.o punto.d.o
O = edg.o err.o memory.o util.o read.o he.o
H = he/edg.h he/err.h he/memory.h he/punto.h he/vec.h he/macro.h he/util.h he/read.h he/off.h he/tri.h he/he.h he/x.h \
he/prec/s/real.h he/prec/d/real.h
I = inc/def.h

PREFIX = $(HOME)
TRG = he
L = libhe_s.a libhe_d.a
P = he_s.pc he_d.pc
HE_CFLAGS = -Wall -Wextra -O2 -g
SED = sed

install: $L $H $P
	mkdir -p -- $(PREFIX)/lib $(PREFIX)/lib
	cp $L    $(PREFIX)/lib
	cp $P    $(PREFIX)/pkgconfig
	for f in $H                  ; \
	do d=`dirname "$$f"`         ; \
	   d=$(PREFIX)/include/"$$d" ; \
	   t=$(PREFIX)/include/"$$f" ; \
	   mkdir -p -- "$$d"         ; \
	   cp -- "$$f" "$$t"         ; \
	done
libhe_s.a: $O $(O_S); ar rv $@ $O $(O_S) && ranlib $@
libhe_d.a: $O $(O_D); ar rv $@ $O $(O_D) && ranlib $@
$O : $H $I
%.o:   %.c; $(CC)                 $(CFLAGS) $(HE_CFLAGS) -c -o $@ $<
%.s.o: %.c; $(CC) -I$(TRG)/prec/s $(CFLAGS) $(HE_CFLAGS) -c -o $@ $<
%.d.o: %.c; $(CC) -I$(TRG)/prec/d $(CFLAGS) $(HE_CFLAGS) -c -o $@ $<
he_s.pc: $(TRG).pc.in
	$(SED) -e "s,@PREFIX@,$(PREFIX),g; s,@PREC@,s,g" $< > $@
he_d.pc: $(TRG).pc.in
	$(SED) -e "s,@PREFIX@,$(PREFIX),g; s,@PREC@,d,g" $< > $@

clean:; rm -f $O $(O_S) $(O_D) $L $P
test:

.PHONY: clean install test
