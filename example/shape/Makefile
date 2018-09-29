include ../../conf.mk

M = main
PREC = d
HE_CFLAGS = -Wall -Wextra -O2 -g `he.conf --cflags $(PREC)`
HE_LDFLAGS = `he.conf --libs $(PREC)`

$M: $M.o; $(CC) $< $(LDFLAGS) $(HE_LDFLAGS) -o $@
$M.o: $M.c; $(CC) $(CFLAGS) $(HE_CFLAGS) $< -c -o $@
$M.o: inc/mesh.h inc/march.h  inc/util.h inc/io.h

test: $M; atest test/*
install:

.PHONY: clean test install
clean:; rm -f $M.o $M
