.SUFFIXES:  .o .s.o .d.o .l.o

install: lib hdr
lib: $L
	co.install $L $(PREFIX)/lib
hdr: $H
	co.install $H $(PREFIX)/include

libalg_s.a: $O $(O_S); ar rv $@ $O $(O_S) && ranlib $@
libalg_d.a: $O $(O_D); ar rv $@ $O $(O_D) && ranlib $@
libalg_l.a: $O $(O_L); ar rv $@ $O $(O_L) && ranlib $@
$O : $H $I

.c.o:; $(CC) $(CFLAGS) $(ALG_CFLAGS) $(CO_S_CFLAGS) -c -o $@ $<
.c.s.o:; $(CC) $(CFLAGS) $(ALG_CFLAGS) $(CO_S_CFLAGS) -c -o $@ $<
.c.d.o:; $(CC) $(CFLAGS) $(ALG_CFLAGS) $(CO_D_CFLAGS) -c -o $@ $<
.c.l.o:; $(CC) $(CFLAGS) $(ALG_CFLAGS) $(CO_L_CFLAGS) -c -o $@ $<

clean:; rm -f $O $(O_S) $(O_D) $(O_L) $L
test:
lint:; $(MAKE) CFLAGS='-O2 -Wall -Wextra' lib$P_d.a

.PHONY: clean hdr install lib lint test
