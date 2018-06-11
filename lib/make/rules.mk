install: $L $H
	he.install $L $(PREFIX)/lib
	he.install $H $(PREFIX)/include

libhe_s.a: $O $(O_S); ar rv $@ $O $(O_S) && ranlib $@
libhe_d.a: $O $(O_D); ar rv $@ $O $(O_D) && ranlib $@
$O : $H $I

%.o:   %.c; $(CC)                 $(CFLAGS) $(HE_CFLAGS) $(GSL_CFLAGS) -c -o $@ $<
%.s.o: %.c; $(CC) -I$(TRG)/prec/s $(CFLAGS) $(HE_CFLAGS) $(GSL_CFLAGS) -c -o $@ $<
%.d.o: %.c; $(CC) -I$(TRG)/prec/d $(CFLAGS) $(HE_CFLAGS) $(GSL_CFLAGS) -c -o $@ $<

clean:; rm -f $O $(O_S) $(O_D) $L
test:

.PHONY: clean install test
