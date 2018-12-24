include ../../conf.mk
M = main
PREC = d
HE_CFLAGS = `he.conf --cflags $(PREC)`
HE_LDFLAGS = `he.conf --libs $(PREC)`

$M: $M.o; $(CC) $< $(LDFLAGS) $(HE_LDFLAGS) -o $@
$M.o: $M.c; $(CC) $(CFLAGS) $(HE_CFLAGS) $< -c -o $@
%.c:   %.m4; he.m4 -o $@ $<

test: $M; atest test/*
install:

.PHONY: clean test install
clean:; rm -f $M.o $M
