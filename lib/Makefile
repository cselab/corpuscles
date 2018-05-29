OFILES = err.o memory.o punto.o vec.o util.o
HFILES = he/err.h he/memory.h he/punto.h he/vec.h he/macro.h he/util.h

PREFIX = $(HOME)
TRG = tri
LIB = lib$(TRG).a
PC  = $(TRG).pc
HE_CFLAGS = -Wall -Wextra -O3 -g
SED = sed

$(LIB): $(OFILES); ar rv $@ $(OFILES) && ranlib $@
$(OFILES) : $(HFILES)

%.o: %.c; $(CC) $(CFLAGS) $(HE_CFLAGS) -c -o $@ $<
kernel.o: kernel/main.h kernel/quintic.h kernel/wendland6.h kernel/yang.h

$(PC): tri.pc.in
	$(SED) -e "s,@PREFIX@,$(PREFIX),g" \
               -e "s,@LIB@,$(LIB),g" \
               -e "s,@TRG@,$(TRG),g" \
        $< > $@

install: $(LIB) $(HFILES) $(PC)
	mkdir -p $(PREFIX)/lib $(PREFIX)/include/$(TRG) $(PREFIX)/pkgconfig
	cp $(LIB)    $(PREFIX)/lib
	cp $(PC)     $(PREFIX)/pkgconfig
	cp $(HFILES) $(PREFIX)/include/$(TRG)

clean:; rm -f $(OFILES) $(LIB) $(PC)
.PHONY: clean install
