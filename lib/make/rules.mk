install: lib hdr
lib: $L; co.install $L $(PREFIX)/lib
hdr: $H; co.install $H $(PREFIX)/include

lib$(P)_s.a: $O $(O_S); ar r $@ $O $(O_S) && ranlib $@
lib$(P)_d.a: $O $(O_D); ar r $@ $O $(O_D) && ranlib $@
lib$(P)_l.a: $O $(O_L); ar r $@ $O $(O_L) && ranlib $@

.c.m4.c:; co.m4 -s -o $@ $<
.h.m4.h:; co.m4 -s -o $@ $<
.f.o:;   $(FC) $< -c $(FCFLAGS) -o $@
.f90.o:; $(FC) $< -c $(FCFLAGS) -o $@
.c.o:;   $(CC) $< -c -I. $(CFLAGS) $(CO_CFLAGS) -o $@
.c.s.o:; $(CC) $< -c -I$P/prec/s -Iinc/s -I. $(CFLAGS) $(CO_CFLAGS) -o $@
.c.d.o:; $(CC) $< -c -I$P/prec/d -Iinc/d -I. $(CFLAGS) $(CO_CFLAGS) -o $@
.c.l.o:; $(CC) $< -c -I$P/prec/l -Iinc/l -I. $(CFLAGS) $(CO_CFLAGS) -o $@

clean:; rm -f $O $(O_S) $(O_D) $(O_L) $L
test:
lint:; $(MAKE) CFLAGS='-Wall -Wextra -O2' lib$(P)_d.a

co/git.h: git
.PHONY: clean install test lint lib hdr git
