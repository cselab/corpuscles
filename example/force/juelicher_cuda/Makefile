include ../../../conf.mk

M = main
PREC = s
HE_CFLAGS =  `he.conf --cflags $(PREC)`
HE_LDFLAGS = `he.conf --libs $(PREC)`

$M: $M.o; $(CC) $< $(LDFLAGS) $(HE_LDFLAGS) -o $@
$M.o: $M.c; $(CC) $(CFLAGS) $(HE_CFLAGAS) $< -c -o $@

test: $M; atest test/*
install:

.PHONY: clean test install
clean:; rm -f $M.o $M
