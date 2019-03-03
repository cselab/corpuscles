.SUFFIXES: .m4

include ../../conf.mk
M = main
PREC = d
HE_CFLAGS = `co.conf --cflags $(PREC)`
HE_LDFLAGS = `co.conf --libs $(PREC)`

$M: $M.o; $(CC) $< $(LDFLAGS) $(HE_LDFLAGS) -o $@
$M.o: $M.c; $(CC) $(CFLAGS) $(HE_CFLAGS) $< -c -o $@

.m4.c:; co.m4 -s -o $@ $<

test: $M; atest test/*
install:

.PHONY: clean test install
clean:; rm -f $M.o $M main.c
