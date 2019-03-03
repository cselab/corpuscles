.SUFFIXES: .c.m4 .h.m4 .s.o .d.o .l.o

install: $L $H
	co.install $L $(PREFIX)/lib
	co.install $H $(PREFIX)/include

libhe_s.a: $O $(O_S); ar r $@ $O $(O_S) && ranlib $@
libhe_d.a: $O $(O_D); ar r $@ $O $(O_D) && ranlib $@
libhe_l.a: $O $(O_L); ar r $@ $O $(O_L) && ranlib $@
$O : $H $I

.c.m4.c:; co.m4 -s -o $@ $<
.h.m4.h:; co.m4 -s -o $@ $<
.c.o:;   $(CC)                         -I. $(CFLAGS) $(HE_CFLAGS) -c -o $@ $<
.c.s.o:; $(CC) -I$(TRG)/prec/s -Iinc/s -I. $(CFLAGS) $(HE_CFLAGS) -c -o $@ $<
.c.d.o:; $(CC) -I$(TRG)/prec/d -Iinc/d -I. $(CFLAGS) $(HE_CFLAGS) -c -o $@ $<
.c.l.o:; $(CC) -I$(TRG)/prec/l -Iinc/l -I. $(CFLAGS) $(HE_CFLAGS) -c -o $@ $<

clean:; rm -f $O $(O_S) $(O_D) $L
test:

.PHONY: clean install test
