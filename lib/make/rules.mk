install: $L $H
	he.install $L $(PREFIX)/lib
	he.install $H $(PREFIX)/include

libhe_s.a: $O $(O_S); ar r $@ $O $(O_S) && ranlib $@
libhe_d.a: $O $(O_D); ar r $@ $O $(O_D) && ranlib $@
libhe_l.a: $O $(O_L); ar r $@ $O $(O_L) && ranlib $@
$O : $H $I

%.c:   %.m4; he.m4 -s $< > /tmp/he.$$.c && mv /tmp/he.$$.c $@
%.o:   %.c; $(CC)                         -I. $(CFLAGS) $(HE_CFLAGS) -c -o $@ $<
%.s.o: %.c; $(CC) -I$(TRG)/prec/s -Iinc/s -I. $(CFLAGS) $(HE_CFLAGS) -c -o $@ $<
%.d.o: %.c; $(CC) -I$(TRG)/prec/d -Iinc/d -I. $(CFLAGS) $(HE_CFLAGS) -c -o $@ $<
%.l.o: %.c; $(CC) -I$(TRG)/prec/l -Iinc/l -I. $(CFLAGS) $(HE_CFLAGS) -c -o $@ $<

clean:; rm -f $O $(O_S) $(O_D) $L
test:

.PHONY: clean install test
