PREFIX = $(HOME)
HE_CFLAGS = -Wall -Wextra -O2 -g

O_S = off.s.o x.s.o vec.s.o tri.s.o dtri.s.o punto.s.o ten.s.o
O_D = off.d.o x.d.o vec.d.o tri.d.o dtri.d.o punto.d.o ten.d.o
O = edg.o err.o memory.o util.o read.o he.o
H = he/edg.h he/err.h he/memory.h he/punto.h he/vec.h he/macro.h he/util.h \
    he/ten.h he/read.h he/off.h he/tri.h he/he.h he/x.h he/prec/s/real.h he/prec/d/real.h
I = inc/def.h
TRG = he
L = libhe_s.a libhe_d.a

install: $L $H
	he.install $L $(PREFIX)/lib
	he.install $H $(PREFIX)/include

libhe_s.a: $O $(O_S); ar rv $@ $O $(O_S) && ranlib $@
libhe_d.a: $O $(O_D); ar rv $@ $O $(O_D) && ranlib $@
$O : $H $I

%.o:   %.c; $(CC)                 $(CFLAGS) $(HE_CFLAGS) -c -o $@ $<
%.s.o: %.c; $(CC) -I$(TRG)/prec/s $(CFLAGS) $(HE_CFLAGS) -c -o $@ $<
%.d.o: %.c; $(CC) -I$(TRG)/prec/d $(CFLAGS) $(HE_CFLAGS) -c -o $@ $<

clean:; rm -f $O $(O_S) $(O_D) $L
test:

.PHONY: clean install test
