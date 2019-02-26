include ../../conf.mk
M = main
PREC = d
HE_CFLAGS = `he.conf --cflags $(PREC)`
HE_LDFLAGS = `he.conf --libs $(PREC)`

$M: $M.c; $(CC) $< $(LDFLAGS) $(HE_CFLAGS) $(HE_LDFLAGS) -o $@

test:
install:

.PHONY: clean test install
clean:; rm -f $M.o $M
