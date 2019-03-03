include ../../conf.mk

M = main
PREC = s
HE_CFLAGS =  `co.conf --cflags $(PREC)`
HE_LDFLAGS = `co.conf --libs $(PREC)`
ALG_LDFLAGS = `alg.conf --libs $(PREC)`

install: $M
	mkdir -p $(PREFIX)/bin
	cp main $(PREFIX)/bin/he.cmoment

$M: $M.o; $(CC) $< $(LDFLAGS) $(HE_LDFLAGS) $(ALG_LDFLAGS) -o $@
$M.o: $M.c; $(CC) $(CFLAGS) $(HE_CFLAGS) $< -c -o $@

test:

.PHONY: clean test install
clean:; rm -f $M.o $M
