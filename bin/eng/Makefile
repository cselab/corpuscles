include ../../conf.mk

M = main
PREC = s
CO_CFLAGS = `co.conf --cflags $(PREC)`
CO_LDFLAGS = `co.conf --libs $(PREC)`

install: $M
	mkdir -p $(PREFIX)/bin
	cp main $(PREFIX)/bin/co.eng

$M: $M.o; $(CC) $< $(LDFLAGS) $(CO_LDFLAGS) -o $@
$M.o: $M.c; $(CC) $(CFLAGS) $(CO_CFLAGS) $< -c -o $@

test:

.PHONY: clean test install
clean:; rm -f $M.o $M
