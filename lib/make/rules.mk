.SUFFIXES: .c.m4 .h.m4 .s.o .d.o .l.o

install: lib hdr
lib: $L; co.install $L $(PREFIX)/lib
hdr: $H; co.install $H $(PREFIX)/include

lib$(P)_s.a: $O $(O_S); ar r $@ $O $(O_S) && ranlib $@
lib$(P)_d.a: $O $(O_D); ar r $@ $O $(O_D) && ranlib $@
lib$(P)_l.a: $O $(O_L); ar r $@ $O $(O_L) && ranlib $@

.c.m4.c:; co.m4 -s -o $@ $<
.h.m4.h:; co.m4 -s -o $@ $<
.c.o:;   $(CC)                     -I. $(CFLAGS) $(CO_CFLAGS) $(LIB_CFLAGS_D) -c -o $@ $<
.c.s.o:; $(CC) -I$P/prec/s -Iinc/s -I. $(CFLAGS) $(CO_CFLAGS) $(LIB_CFLAGS_S) -c -o $@ $<
.c.d.o:; $(CC) -I$P/prec/d -Iinc/d -I. $(CFLAGS) $(CO_CFLAGS) $(LIB_CFLAGS_D) -c -o $@ $<
.c.l.o:; $(CC) -I$P/prec/l -Iinc/l -I. $(CFLAGS) $(CO_CFLAGS) $(LIB_CFLAGS_L) -c -o $@ $<

clean:; rm -f $O $(O_S) $(O_D) $(O_L) $L
test:
lint:; $(MAKE) CFLAGS='-Wall -Wextra -O2' lib$(P)_d.a

co/git.h: git
.PHONY: clean install test lint lib hdr git
