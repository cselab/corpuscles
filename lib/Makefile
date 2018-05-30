OFILES_S = off.s.o
OFILES = err.o memory.o punto.o vec.o util.o read.o he.o x.o
HFILES = he/err.h he/memory.h he/punto.h he/vec.h he/macro.h he/util.h he/read.h he/he.h he/x.h \
he/prec/s/real.h he/prec/d/real.h
DFILES = inc/def.h

PREFIX = $(HOME)
TRG = he
LIB = libhe_s.a libhe_d.a
PC  = he_s.pc he_d.pc
HE_CFLAGS = -Wall -Wextra -O2 -g
SED = sed

install: $(LIB) $(HFILES) $(PC)
	mkdir -p -- $(PREFIX)/lib $(PREFIX)/lib
	cp $(LIB)    $(PREFIX)/lib
	cp $(PC)     $(PREFIX)/pkgconfig
	for f in $(HFILES)           ; \
	do d=`dirname "$$f"`         ; \
	   d=$(PREFIX)/include/"$$d" ; \
           t=$(PREFIX)/include/"$$f" ; \
	   mkdir -p -- "$$d"         ; \
	   cp -- "$$f" "$$t"         ; \
	done
libhe_s.a: $(OFILES) $(OFILES_S); ar rv $@ $< && ranlib $@
libhe_d.a: $(OFILES) $(OFILES_D); ar rv $@ $< && ranlib $@
$(OFILES) : $(HFILES) $(DFILES)
%.o: %.c;   $(CC)                 $(CFLAGS) $(HE_CFLAGS) -c -o $@ $<
%.s.o: %.c; $(CC) -I$(TRG)/prec/s $(CFLAGS) $(HE_CFLAGS) -c -o $@ $<
%.d.o: %.c; $(CC) -I$(TRG)/prec/d $(CFLAGS) $(HE_CFLAGS) -c -o $@ $<
he_s.pc: $(TRG).pc.in
	$(SED) -e "s,@PREFIX@,$(PREFIX),g; s,@PREC@,s,g" $< > $@
he_d.pc: $(TRG).pc.in
	$(SED) -e "s,@PREFIX@,$(PREFIX),g; s,@PREC@,d,g" $< > $@

clean:; rm -f $(OFILES) $(LIB) $(PC)
test:

.PHONY: clean install test
