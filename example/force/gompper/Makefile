include ../../../conf.mk

M = main
PREC = d
CO_CFLAGS = `co.conf --cflags $(PREC)`
CO_LDFLAGS = `co.conf --libs $(PREC)`

$M: $M.o; $(CC) $< $(LDFLAGS) $(CO_LDFLAGS) -o $@
$M.o: $M.c; $(CC) $(CFLAGS) $(CO_CFLAGS) $< -c -o $@
$M.o: inc/main.h  inc/mesh.h  inc/util.h

test: $M; atest test/*
install:

.PHONY: clean test install
clean:; rm -f $M.o $M
