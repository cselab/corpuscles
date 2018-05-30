OFILES = err.o memory.o punto.o vec.o util.o read.o he.o x.o
HFILES = he/err.h he/memory.h he/punto.h he/vec.h he/macro.h he/util.h he/read.h he/he.h he/x.h \
he/single/real.h he/double/real.h
DFILES = inc/def.h

PREFIX = $(HOME)
TRG = he
LIB = lib$(TRG).a
PC  = $(TRG).pc
HE_CFLAGS = -Wall -Wextra -O2 -g
SED = sed

$(LIB): $(OFILES); ar rv $@ $(OFILES) && ranlib $@
$(OFILES) : $(HFILES) $(DFILES)
%.o: %.c; $(CC) $(CFLAGS) $(HE_CFLAGS) -c -o $@ $<

$(PC): $(PC).in
	$(SED) -e "s,@PREFIX@,$(PREFIX),g" \
               -e "s,@LIB@,$(LIB),g" \
               -e "s,@TRG@,$(TRG),g" \
        $< > $@

install: $(LIB) $(HFILES) $(PC)
	cp $(LIB)    $(PREFIX)/lib
	cp $(PC)     $(PREFIX)/pkgconfig
	for f in $(HFILES)           ; \
	do d=`dirname "$$f"`         ; \
	   d=$(PREFIX)/include/"$$d" ; \
           t=$(PREFIX)/include/"$$f" ; \
	   mkdir -p -- "$$d"         ; \
	   cp -- "$$f" "$$t"         ; \
	done

clean:; rm -f $(OFILES) $(LIB) $(PC)
test:

.PHONY: clean install test
