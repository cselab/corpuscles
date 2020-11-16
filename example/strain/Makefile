include ../../conf.mk

M = main
PREC = d
CO_CFLAGS = `co.conf --cflags $(PREC)`
CO_LDFLAGS = `co.conf --libs $(PREC)`

$M: $M.o; $(CC) $< $(LDFLAGS) $(CO_LDFLAGS) -o $@
$M.o: $M.c; $(CC) $(CFLAGS) $(CO_CFLAGS) $< -c -o $@

test: $M
install:

.PHONY: clean test install
clean:; rm -f $M.o $M
