OFILES = err.o memory.o punto.o vec.o util.o read.o he.o x.o
HFILES = he/err.h he/memory.h he/punto.h he/vec.h he/macro.h he/util.h he/read.h he/he.h he/x.h \
he/single/real.h he/double/real.h
# subdirectories in HFILES
DIRS   = prec
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

dir:
	mkdir -p $(PREFIX)/lib $(PREFIX)/include/$(TRG) $(PREFIX)/pkgconfig
	for d in $(DIRS); \
        do mkdir -p $(PREFIX)/include/$(TRG)/"$$d"; \
        done

install: $(LIB) $(HFILES) $(PC) dir
	cp $(LIB)    $(PREFIX)/lib
	cp $(PC)     $(PREFIX)/pkgconfig
	cp $(HFILES) $(PREFIX)/include/$(TRG)

clean:; rm -f $(OFILES) $(LIB) $(PC)
test:

.PHONY: clean install test dir
